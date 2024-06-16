/*
 * app.cpp
 *
 *  Created on: Jun 12, 2024
 *      Author: monica
 */



int k = 1;

float Adata[2*2] = { 0.9324, 0, 0, 0.9324 };
float Bdata[2*3] = { 0.6439, -0.3219, -0.3219, 0, 0.5576, -0.5576 };
float xkdata[2 * 1] = { 0.0, 0.0 };
float ukdata[1*3] = { 0.0, 0.0, 0.0 };
float xrefdata[2*1] = { 5, 5 };
Matrix_t A = {2, 2, Adata};
Matrix_t B = {2, 3, Bdata};

Matrix_t xk = {2, 1, xkdata};
Matrix_t uk = {3, 1, ukdata};
Matrix_t x_ref = {2, 1, xrefdata};

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

unsigned int selectBestCombination();

void app()
{
    // Parameters for the loop
    int k_test = 50;

	unsigned int bestCombination;

    // equation
    /*
    Phase A
    y = 11.2 x - 18.094

    Phase B
    y = 11.795 x - 19.049
    */
    for ( k = 1; k < k_test; ++k )
    {
		bestCombination = selectBestCombination();
		memcpy(uk.data, switch_state[bestCombination], 3 * sizeof(float));

		// Propagate system state by doing
		// xk = A * xk + B * uk
		matrixMultiply(&A, &xk, &xk);                      // xk = A * xk  
		matrixMultiplyAddingToResult(&B, &uk, &xk);        // xk = xk + B * uk
	}
}

unsigned int selectBestCombination() {
	float guk;
	float lambda = 0.01;   // switch

	float g_idata[2*1]= {0, 0};
	Matrix_t g_i = {2, 1, g_idata};

	float g_udata[1*3]= {0, 0, 0};
	Matrix_t g_u = {3, 1, g_udata};

	float x_pdata[2 * 1] = { 0, 0 };
	Matrix_t x_p = { 2, 1, x_pdata };

	float u_pdata[3 * 1] = { 0, 0, 0 };
	Matrix_t u_p = { 3, 1, u_pdata };

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

