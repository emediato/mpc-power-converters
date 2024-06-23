
#include "app.h"
//#include "tim.h"
//#include "FreeRTOS.h"

#define FRACTIONAL_BITS 12 // Q factor for fixed points
#define SCALING_FACTOR ((fixed_point_t)1 << FRACTIONAL_BITS)
#define ITERATIONS 20  // Number of iterations for the Taylor series
#define SCALE_FACTOR 10000  // Scaling factor to manage large numbers


uint16_t adcRawValues[2];

float Ts = 1/1000; // seconds
matrix_type Ts_fixed = float_to_fixed(Ts); // seconds
float Vd = 60; // voltage
matrix_type V_fixed = float_to_fixed(Vd);
float R = 22+3; //   ohms
float L = 0.10 ;
float g_i_norm = 0.01;
float min_g = 1000000.0;


matrix_type g_i_fixed = float_to_fixed(g_i_norm);
matrix_type min_g_fixed = float_to_fixed(min_g);
matrix_type L_fixed = float_to_fixed(L); // henry

matrix_type x_ref_fixed[2*2];
matrix_type A_fixed[2*2] = { 0, 0, 0, 0};
matrix_type Adata[2*2] = { 0, 0, 0, 0};
matrix_type I_2_fixed[2*2];
matrix_type I_2_data[2*2] = { 1, 0, 0, 1 };
matrix_type F_data[2*2] = { 0, 0, 0, 0};
matrix_type F_Ts_data[2*2] =  { 0, 0, 0, 0};
matrix_type G_data[2*3] = { 0, 0, 0, 0, 0, 0};

matrix_type Fdata_fixed[2 * 3];
matrix_type K_fixed[2 * 3];
matrix_type Bdata[2*3];
matrix_type Bdata_fixed[2 * 3];


float x_ref[2*1] = {.5 , .5} ;
float Kdata[2*3] = {0.6667, -0.3333, -0.3333, 0, 0.5774, -0.5774 };


float Adata[2*2] = {0.7985, 0, 0,  0.7985};
float Bdata[2*3] = {0.1791, -0.0895, -0.0895, 0, 0.1551, -0.1551};
float xkdata[2 * 1] = { 0.0, 0.0 };
float ukdata[1*3] = { 0.0, 0.0, 0.0 };
float xrefdata[2*1] = { 0.5, 0.5 };
float idata[3*1];

int ialfa[200]; int ibeta[200]; volatile int kindex;

matrix_t A = {2, 2, Adata};
matrix_t B = {2, 3, Bdata};
matrix_t K = {2, 3, Kdata};
matrix_t Iabc = {3, 1, idata};

matrix_t xk = {2, 1, xkdata};
matrix_t uk = {3, 1, ukdata};
matrix_t x_ref = {2, 1, xrefdata};

#define NumberOfStates 7
const float switch_state[NumberOfStates][3] = {
    {0, 0, 0},  // 0
    {0, 0, 1},  // 1
    {0, 1, 0},  // 2
    {0, 1, 1},  // 3
    {1, 0, 0},  // 4
    {1, 1, 0},  // 5
    {1, 0, 1}   // 6
};

void App()
{
	// SIMULATING ADC
	Iabc.data[0] = 0.0090 * ((float) 1) - 18.094;
	Iabc.data[1] = 0.0095 * ((float) 2) - 19.049;
	Iabc.data[2] = Iabc.data[0] - Iabc.data[1];

	/*

	//3.3 / 4096 = 8.0566e-04
	// 8.0566e-04 * 11.2 =  0.0090
	//  8.0566e-04 * 11.2795 = 0.0095

	Iabc.data[0] = 0.0090 * ((float) adcRawValues[1]) - 18.094;
	Iabc.data[1] = 0.0095 * ((float) adcRawValues[0]) - 19.049;
	Iabc.data[2] = Iabc.data[0] - Iabc.data[1];
	*/

	// xk = K * Iabc
	matrixMultiply(&K, &Iabc, &xk);

	// Calculate best combination
	unsigned int bestCombination = selectBestCombination();
	memcpy(uk.data, switch_state[bestCombination], 3 * sizeof(float));


	if (kindex < 198)
	{
		ialfa[kindex] = (int) 1000 * xkdata[0];
		ibeta[kindex] = (int) 1000 * xkdata[1];
		switch_all[kindex] = bestCombination;
	}
	kindex++;
}


unsigned int selectBestCombination() {
	float guk;
	float lambda = 0.01;   // switch

	float g_idata[2*1]= {0, 0};
	matrix_t g_i = {2, 1, g_idata};

	float g_udata[1*3]= {0, 0, 0};
	matrix_t g_u = {3, 1, g_udata};

	float x_pdata[2 * 1] = { 0, 0 };
	matrix_t x_p = { 2, 1, x_pdata };

	float u_pdata[3 * 1] = { 0, 0, 0 };
	matrix_t u_p = { 3, 1, u_pdata };

	float min_g = 1000000.0;
	int min_index = 0;
	float g_i_norm = 0.0;
	float g_u_norm = 0.0;

	for (unsigned int combination = 0; combination < NumberOfStates; ++combination)
	{
		// Get the actual combination to be tested
		memcpy(u_p.data, switch_state[combination], 3 * sizeof(float));

		// Predict the next system state using this control combination
		// x_p = A * xk + B * u_p
		matrixMultiply(&A, &xk, &x_p);                   // x_p = A * xk
		matrixMultiplyAddingToResult(&B, &u_p, &x_p);    // x_p = x_p + B * u_p

		// Calculate the deviation to the desired system state and normalize it
		matrixSubtract(&x_ref, &x_p, &g_i);              // g_i = x_ref - x_p;
		g_i_norm = matrixNorm2(&g_i);                    // g_i_norm = g_i.norm()

		// Calculate how many switches changed
		matrixSubtract(&u_p, &uk, &g_u);                 // g_u = u_p - uk;
		g_u_norm = matrixNorm2_2(&g_u);

		// Calculate the overall cost
		guk = g_i_norm + g_u_norm * lambda;

		if (guk < min_g )
		{
			min_g = guk;
			min_index = combination;
		}
	}
	return min_index;
}

