#include <stdio.h>

#include "matrix.h"
#include <stdio.h>
#include <stddef.h>
#include <string.h>

#include "main.h"
#include "app.h"


/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include <string.h>
#include "matrix.h"
#include "time.h"

uint16_t adcRawValues[2];


int ialfa[200]; int ibeta[200]; int kindex; int switch_all[200];
matrix_t Iabc;

int ialfa[200]; int ibeta[200];



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

int main(void) {

    printf("Hello, Entering App !\n");

    App();


    char buffer[20];

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */

    while (1)
    {
        /* USER CODE END WHILE */

        /* USER CODE BEGIN 3 */
        /*
        if (kindex >= 198){
            for (int amostra = 0; amostra < 198; amostra++)
            {
                sprintf(buffer, "%d, %d;", ialfa[amostra], ibeta[amostra]);
                CDC_Transmit_FS((uint8_t*) buffer, strlen(buffer));
                HAL_Delay(2);
            }

            while(1) {};

        }*/


    }
    return 0;
}



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


//
//
// void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
// {
// 	Iabc.data[0] = 0.0090 * ((float) adcRawValues[1]) - 18.094;
// 	Iabc.data[1] = 0.0095 * ((float) adcRawValues[0]) - 19.049;
// 	Iabc.data[2] = Iabc.data[0] - Iabc.data[1];
//
// 	// xk = K * Iabc
// 	matrixMultiply(&K, &Iabc, &xk);
//
// 	if (kindex < 198)
// 	{
// 		ialfa[kindex] = (int) 1000 * xkdata[0];
// 		ibeta[kindex] = (int) 1000 * xkdata[1];
// 	}
// 	kindex++;
//
// 	// Calculate best combination
// 	unsigned int bestCombination = selectBestCombination();
// 	memcpy(uk.data, switch_state[bestCombination], 3 * sizeof(float));
//
// 	// First make sure all switches are turned off
// 	HAL_GPIO_WritePin(DRIVE1_LOW_GPIO_Port, DRIVE1_LOW_Pin, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin(DRIVE2_LOW_GPIO_Port, DRIVE2_LOW_Pin, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin(DRIVE3_LOW_GPIO_Port, DRIVE3_LOW_Pin, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin(DRIVE1_HIGH_GPIO_Port, DRIVE1_HIGH_Pin, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin(DRIVE2_HIGH_GPIO_Port, DRIVE2_HIGH_Pin, GPIO_PIN_RESET);
// 	HAL_GPIO_WritePin(DRIVE3_HIGH_GPIO_Port, DRIVE3_HIGH_Pin, GPIO_PIN_RESET);
//
// 	// Dead time
// 	//	int i = 100;
// 	//	while (i > 1) { i--; }
// 	delayUs(2);
//
// 	if (switch_state[bestCombination][0] > 0)
// 		HAL_GPIO_WritePin(DRIVE1_HIGH_GPIO_Port, DRIVE1_HIGH_Pin, GPIO_PIN_SET);
// 	else
// 		HAL_GPIO_WritePin(DRIVE1_LOW_GPIO_Port, DRIVE1_LOW_Pin, GPIO_PIN_SET);
//
// 	if (switch_state[bestCombination][1] > 0)
// 		HAL_GPIO_WritePin(DRIVE2_HIGH_GPIO_Port, DRIVE2_HIGH_Pin, GPIO_PIN_SET);
// 	else
// 		HAL_GPIO_WritePin(DRIVE2_LOW_GPIO_Port, DRIVE2_LOW_Pin, GPIO_PIN_SET);
//
// 	if (switch_state[bestCombination][2] > 0)
// 		HAL_GPIO_WritePin(DRIVE3_HIGH_GPIO_Port, DRIVE3_HIGH_Pin, GPIO_PIN_SET);
// 	else
// 		HAL_GPIO_WritePin(DRIVE3_LOW_GPIO_Port, DRIVE3_LOW_Pin, GPIO_PIN_SET);
// }
