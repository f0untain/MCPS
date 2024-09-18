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
#include "stm32f3xx_hal.h"

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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_BKL_Pin GPIO_PIN_13
#define LCD_BKL_GPIO_Port GPIOC
#define KEY6_Pin GPIO_PIN_14
#define KEY6_GPIO_Port GPIOC
#define LCD4_Pin GPIO_PIN_0
#define LCD4_GPIO_Port GPIOC
#define LCD5_Pin GPIO_PIN_1
#define LCD5_GPIO_Port GPIOC
#define LCD6_Pin GPIO_PIN_2
#define LCD6_GPIO_Port GPIOC
#define LCD7_Pin GPIO_PIN_3
#define LCD7_GPIO_Port GPIOC
#define LCD_EN_Pin GPIO_PIN_2
#define LCD_EN_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_7
#define LCD_RS_GPIO_Port GPIOA
#define BUZZ_Pin GPIO_PIN_10
#define BUZZ_GPIO_Port GPIOB
#define rotaryA_Pin GPIO_PIN_8
#define rotaryA_GPIO_Port GPIOA
#define rotaryB_Pin GPIO_PIN_9
#define rotaryB_GPIO_Port GPIOA
#define rotaryMiddle_Pin GPIO_PIN_10
#define rotaryMiddle_GPIO_Port GPIOA
#define Key4_Pin GPIO_PIN_11
#define Key4_GPIO_Port GPIOA
#define Key5_Pin GPIO_PIN_12
#define Key5_GPIO_Port GPIOA
#define Madule_LED_Pin GPIO_PIN_15
#define Madule_LED_GPIO_Port GPIOA
#define LED1_Pin GPIO_PIN_10
#define LED1_GPIO_Port GPIOC
#define LED2_Pin GPIO_PIN_11
#define LED2_GPIO_Port GPIOC
#define LED3_Pin GPIO_PIN_12
#define LED3_GPIO_Port GPIOC
#define LED4_Pin GPIO_PIN_2
#define LED4_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
