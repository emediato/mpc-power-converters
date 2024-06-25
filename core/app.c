
#include "app.h"
//#include "tim.h"
//#include "FreeRTOS.h"

int k = 1;


matrix_type idata[3*1];
matrix_type Adata_fixed[2*2] = { 3190, 0, 0, 3190};
matrix_type Bdata_fixed[2 * 3] = { 42199, -21096, -21096, 0, 36543, -36543};
matrix_type Kdata_fixed[2 * 3] = { 2731, -1365, -1365, 0, 2365, -2365 };
matrix_type xrefdata_fixed[2*1] = {20480, 20480};
matrix_type xkdata[2 * 1] = { 0, 0 };
matrix_type ukdata[1 * 3] = { 0, 0 };

matrix_t A = {2, 2, Adata_fixed};
matrix_t B = {2, 3, Bdata_fixed};
matrix_t xk = {2, 1, xkdata};
matrix_t uk = {3, 1, ukdata};
matrix_t x_ref = {2, 1, xrefdata_fixed};

matrix_t K = {2,3, Kdata_fixed};

const matrix_type switch_state[NumberOfStates][3] = {
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
	int min_index = 0;


	matrix_type guk;
	matrix_type lambda = 41;   // switch

	matrix_type g_idata[2*1]= {0, 0};
	matrix_t g_i = {2, 1, g_idata};

	matrix_type g_udata[1*3]= {0, 0, 0};
	matrix_t g_u = {3, 1, g_udata};

	matrix_type x_pdata[2 * 1] = { 0, 0 };
	matrix_t x_p = { 2, 1, x_pdata };

	matrix_type u_pdata[3 * 1] = { 0, 0, 0 };
	matrix_t u_p = { 3, 1, u_pdata };

	matrix_type min_g = 2147483647;
	matrix_type g_i_norm = 0;
	matrix_type g_u_norm = 0;

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



