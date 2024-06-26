/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
// #include "stm32f1xx_hal.h"
#include "app.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

#include <stdio.h>
#include <stddef.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

extern uint16_t adcRawValues[2];


extern int ialfa[200];
extern int ibeta[200];
extern int switch_all[200];


extern void App();
unsigned int selectBestCombination();
/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define DRIVE1_LOW_Pin GPIO_PIN_13
#define DRIVE1_LOW_GPIO_Port GPIOB
#define DRIVE2_LOW_Pin GPIO_PIN_14
#define DRIVE2_LOW_GPIO_Port GPIOB
#define DRIVE3_LOW_Pin GPIO_PIN_15
#define DRIVE3_LOW_GPIO_Port GPIOB
#define DRIVE1_HIGH_Pin GPIO_PIN_8
#define DRIVE1_HIGH_GPIO_Port GPIOA
#define DRIVE2_HIGH_Pin GPIO_PIN_9
#define DRIVE2_HIGH_GPIO_Port GPIOA
#define DRIVE3_HIGH_Pin GPIO_PIN_10
#define DRIVE3_HIGH_GPIO_Port GPIOA

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#endif /* __MAIN_H */
