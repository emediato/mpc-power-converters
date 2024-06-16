
#include "app.h"
#include "tim.h"
//#include "FreeRTOS.h"


uint16_t adcRawValues[2];

float Adata[2*2] = {0.7985, 0, 0,  0.7985};
float Bdata[2*3] = {0.1791, -0.0895, -0.0895, 0, 0.1551, -0.1551};
float xkdata[2 * 1] = { 0.0, 0.0 };
float ukdata[1*3] = { 0.0, 0.0, 0.0 };
float xrefdata[2*1] = { 0.5, 0.5 };
float Kdata[2*3] = {0.6667, -0.3333, -0.3333, 0 , 0.5774,-0.5774};
float idata[3*1];

int ialfa[200]; int ibeta[200]; int kindex;

Matrix_t A = {2, 2, Adata};
Matrix_t B = {2, 3, Bdata};
Matrix_t K = {2, 3, Kdata};
Matrix_t Iabc = {3, 1, idata};

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
void HAL_ADC_ConvCpltCallback();
void App();

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

	if (kindex < 198)
	{
		ialfa[kindex] = (int) 1000 * xkdata[0];
		ibeta[kindex] = (int) 1000 * xkdata[1];
	}
	kindex++;

	// Calculate best combination
	unsigned int bestCombination = selectBestCombination();
	memcpy(uk.data, switch_state[bestCombination], 3 * sizeof(float));
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


void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	Iabc.data[0] = 0.0090 * ((float) adcRawValues[1]) - 18.094;
	Iabc.data[1] = 0.0095 * ((float) adcRawValues[0]) - 19.049;
	Iabc.data[2] = Iabc.data[0] - Iabc.data[1];

	// xk = K * Iabc
	matrixMultiply(&K, &Iabc, &xk);

	if (kindex < 198)
	{
		ialfa[kindex] = (int) 1000 * xkdata[0];
		ibeta[kindex] = (int) 1000 * xkdata[1];
	}
	kindex++;

	// Calculate best combination
	unsigned int bestCombination = selectBestCombination();
	memcpy(uk.data, switch_state[bestCombination], 3 * sizeof(float));

	// First make sure all switches are turned off
	HAL_GPIO_WritePin(DRIVE1_LOW_GPIO_Port, DRIVE1_LOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVE2_LOW_GPIO_Port, DRIVE2_LOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVE3_LOW_GPIO_Port, DRIVE3_LOW_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVE1_HIGH_GPIO_Port, DRIVE1_HIGH_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVE2_HIGH_GPIO_Port, DRIVE2_HIGH_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(DRIVE3_HIGH_GPIO_Port, DRIVE3_HIGH_Pin, GPIO_PIN_RESET);

	// Dead time
	//	int i = 100;
	//	while (i > 1) { i--; }
	delayUs(2);

	if (switch_state[bestCombination][0] > 0)
		HAL_GPIO_WritePin(DRIVE1_HIGH_GPIO_Port, DRIVE1_HIGH_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DRIVE1_LOW_GPIO_Port, DRIVE1_LOW_Pin, GPIO_PIN_SET);

	if (switch_state[bestCombination][1] > 0)
		HAL_GPIO_WritePin(DRIVE2_HIGH_GPIO_Port, DRIVE2_HIGH_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DRIVE2_LOW_GPIO_Port, DRIVE2_LOW_Pin, GPIO_PIN_SET);

	if (switch_state[bestCombination][2] > 0)
		HAL_GPIO_WritePin(DRIVE3_HIGH_GPIO_Port, DRIVE3_HIGH_Pin, GPIO_PIN_SET);
	else
		HAL_GPIO_WritePin(DRIVE3_LOW_GPIO_Port, DRIVE3_LOW_Pin, GPIO_PIN_SET);
}