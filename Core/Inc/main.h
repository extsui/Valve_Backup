/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stm32l0xx_hal.h"

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

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RotaryEncoder4A_Pin GPIO_PIN_14
#define RotaryEncoder4A_GPIO_Port GPIOC
#define RotaryEncoder4B_Pin GPIO_PIN_15
#define RotaryEncoder4B_GPIO_Port GPIOC
#define RotaryEncoder1A_Pin GPIO_PIN_3
#define RotaryEncoder1A_GPIO_Port GPIOA
#define RotaryEncoder1B_Pin GPIO_PIN_4
#define RotaryEncoder1B_GPIO_Port GPIOA
#define RotaryEncoder2A_Pin GPIO_PIN_5
#define RotaryEncoder2A_GPIO_Port GPIOA
#define RotaryEncoder2B_Pin GPIO_PIN_6
#define RotaryEncoder2B_GPIO_Port GPIOA
#define RotaryEncoder3A_Pin GPIO_PIN_7
#define RotaryEncoder3A_GPIO_Port GPIOA
#define RotaryEncoder3B_Pin GPIO_PIN_1
#define RotaryEncoder3B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
