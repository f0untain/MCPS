
/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <clockconfig.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "lcd.h"
#include "button.h"
#include "Menu.h"
#include "eeprom.h"
#include "pwm.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
I2C_HandleTypeDef hi2c1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim7;

/* USER CODE BEGIN PV */
volatile bool	
				flag1ms,
				flag10ms,
				flag100ms,
	            flag300ms,
	            flag1s,
	
				flcd10ms,
				flcd100ms,
				f2lcd100ms,
	            flcd300ms,
				flcd500ms,	
	            flcd1s,
	
                flagRefreshValue500ms,	
				flagUpdatePWM,
				flagSetPWM,
				flagCountRamp,
				flagRotaryCW = false,
				flagRotaryCCW = false;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM7_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

//rotary CW/CCW diagnose variables
int32_t encoder_counter = 0;
int32_t last_count = 0;
int32_t diff = 0;

volatile uint8_t /* buttons_release, */ buttonMask;

/* Array to determine that the buzzer need to toggle.
 * this array consists of buzzer[0] = number of toggles
 *					  and buzzer[1] = delay between each toggle.
 * this array must be initialized when need to buzzer toggled.
 */
volatile uint8_t buzzer[2];

volatile uint8_t nextState; // volatile for avoid of optimizing for debug program

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */
	Clock::GetInstance();
  /* USER CODE END 1 */
 

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM7_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
  /*Lcd* LcdInstance = Lcd::GetInstance();
	char data[2] = { 'A', 'B' };
  LcdInstance->Puts(0,0,data);*/
	
	//menu.InitValueWriteToEeprom();
	menu.InitValueReadFromEeprom();
	
	//start encoder timer 1 for reading rotary encoder
	HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);

	//create different flags base on 1ms timer 7
	//__HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE );
	HAL_TIM_Base_Start_IT(&htim7);

	//create PWM and change Frequency and Duty Cycle
	__HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE);
	//HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
	//Set_Frequency(Frequency);
	//Set_DutyCycle(DutyCycle);

	volatile char input; // holds the current keys state.
	uint8_t j;

	char(*pStateFunc)(char);
	// Initial state variables
	uint8_t state = ST_STANDBY;
	//pStateFunc = menu.StandBy;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  //HAL_GPIO_TogglePin(GPIOC, LED1_Pin | LED2_Pin | LED3_Pin);
	  //HAL_Delay(1000);
	  encoder_counter = __HAL_TIM_GET_COUNTER(&htim1) / 4;
	  diff = encoder_counter - last_count; // Calculate the difference between the current count and the last count

	  if (diff != 0)
	  {
		  if (diff > 0)
		  {
			  flagRotaryCW = true; //Clockwise rotation
			  flagRotaryCCW = false;
		  }
		  else if (diff < 0)
		  {
			  flagRotaryCW = false;
			  flagRotaryCCW = true; //Counter-clockwise rotation
		  }

		  last_count = encoder_counter; //Update the last count
	  }

	  if (flag10ms == true)
	  {
		  button.Debounce(); // Update button_state.

		 // if (buzzer[BUZZ_NUM])
		  	//hal.ToggleBuzzer(buzzer);

		  flag10ms = false;
	  }

	  if (flag1ms == true && flagSetPWM == true)
	  {
		  pwm.CountRamp++;
		  if (pwm.CountRamp > pwm.StepRamp)
		  {
			  pwm.CountRamp = 0;
			  flagSetPWM = false;
			  //Set_DutyCycle(DutyCycle);
		  }
		  else
			  pwm.SetDutyCycle((pwm.Time_ON / pwm.StepRamp) * pwm.CountRamp);

		  flag1ms = false;
	  }

	  // Read input and change states
	  buttonMask = KEY_MASK;
	  buttonMask &= button.Release;
	  button.Release ^= buttonMask;
	  input = buttonMask | ((flagRotaryCCW*ROTARY_CCW_DIR)) | ((flagRotaryCW*ROTARY_CW_DIR)); // Read buttons
	  flagRotaryCW = false;
	  flagRotaryCCW = false;


	  //Set buzzer to beep if one button is pressed.
	  if (input)
	  {
		  buzzer[BUZZ_NUM] = 1;
		  buzzer[BUZZ_DELAY] = 10;
	  }

	  //if (tapChangerState == 0 || errorVar != 0)
	  //{
		  // When in this state, we must call the state function
		  nextState = pStateFunc(input);
		  if (menu.errorVar != 0)
			  nextState = ST_ERRORS;
		  if (nextState != state)
		  {
			  state = nextState;

			  for (uint8_t n = 0; (j = menu.StateMenu[n].state); n++)
			  {
				  if (j == state)
				  {
					  pStateFunc = menu.StateMenu[n].pFunc;
					  break;
				  }
			  }

		  }
	  //}	  

    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}


/**
  * @brief I2C1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C1_Init(void)
{

  /* USER CODE BEGIN I2C1_Init 0 */

  /* USER CODE END I2C1_Init 0 */

  /* USER CODE BEGIN I2C1_Init 1 */

  /* USER CODE END I2C1_Init 1 */
  hi2c1.Instance = I2C1;
  hi2c1.Init.Timing = 0x00201826;
  hi2c1.Init.OwnAddress1 = 0;
  hi2c1.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c1.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c1.Init.OwnAddress2 = 0;
  hi2c1.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c1.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c1.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c1, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c1, 5) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C1_Init 2 */

  /* USER CODE END I2C1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_Encoder_InitTypeDef sConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 0;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 65535;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  sConfig.EncoderMode = TIM_ENCODERMODE_TI12;
  sConfig.IC1Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC1Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC1Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC1Filter = 5;
  sConfig.IC2Polarity = TIM_ICPOLARITY_FALLING;
  sConfig.IC2Selection = TIM_ICSELECTION_DIRECTTI;
  sConfig.IC2Prescaler = TIM_ICPSC_DIV1;
  sConfig.IC2Filter = 5;
  if (HAL_TIM_Encoder_Init(&htim1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 7200-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 1000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief TIM7 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM7_Init(void)
{

  /* USER CODE BEGIN TIM7_Init 0 */

  /* USER CODE END TIM7_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM7_Init 1 */

  /* USER CODE END TIM7_Init 1 */
  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 720-1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 1000-1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM7_Init 2 */

  /* USER CODE END TIM7_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStruct = { 0 };
	/* USER CODE BEGIN MX_GPIO_Init_1 */
	/* USER CODE END MX_GPIO_Init_1 */

	  /* GPIO Ports Clock Enable */
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOF_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOC,
		LCD_BKL_Pin|LCD4_Pin|LCD5_Pin|LCD6_Pin
	                        |LCD7_Pin|LED1_Pin|LED2_Pin|LED3_Pin,
		GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(GPIOA, LCD_EN_Pin | LCD_RS_Pin | Madule_LED_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pin Output Level */
	HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

	/*Configure GPIO pins : LCD_BKL_Pin LED1_Pin LED2_Pin LED3_Pin */
	GPIO_InitStruct.Pin = LCD_BKL_Pin | LED1_Pin | LED2_Pin | LED3_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pin : KeyStart_Pin */
	GPIO_InitStruct.Pin = KeyStart_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(KeyStart_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : LCD4_Pin LCD5_Pin LCD6_Pin LCD7_Pin */
	GPIO_InitStruct.Pin = LCD4_Pin | LCD5_Pin | LCD6_Pin | LCD7_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	/*Configure GPIO pins : LCD_EN_Pin LCD_RS_Pin */
	GPIO_InitStruct.Pin = LCD_EN_Pin | LCD_RS_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : Buzzer_Pin */
	GPIO_InitStruct.Pin = Buzzer_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Buzzer_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pins : KeyRotaryMiddle_Pin KeyStandby_Pin KeyBack_Pin */
	GPIO_InitStruct.Pin = KeyRotaryMiddle_Pin | KeyStandby_Pin | KeyBack_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	GPIO_InitStruct.Pull = GPIO_PULLUP;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	/*Configure GPIO pin : Madule_LED_Pin */
	GPIO_InitStruct.Pin = Madule_LED_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(Madule_LED_GPIO_Port, &GPIO_InitStruct);

	/*Configure GPIO pin : LED4_Pin */
	GPIO_InitStruct.Pin = LED4_Pin;
	GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	GPIO_InitStruct.Pull = GPIO_PULLDOWN;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(LED4_GPIO_Port, &GPIO_InitStruct);

	/* USER CODE BEGIN MX_GPIO_Init_2 */
	/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */
/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM6 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM6) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */
	if (htim->Instance == TIM7) {
		volatile static uint32_t count1ms = 0;
		count1ms++;
		flag1ms = true;

		if ((count1ms % 10) == 0)
		{
			//if ( (count1ms % 10) == 0 )
			//{
			flag10ms = true;
			flcd10ms = true;
			//}
			if ((count1ms % 100) == 0)
			{
				flag100ms = true;
				flcd100ms = true;
				f2lcd100ms = true;
			}
			if ((count1ms % 300) == 0)
			{
				flag300ms = true;
				flcd300ms = true;
			}
			if ((count1ms % 500) == 0)
			{
				flcd500ms = true;

			}
			if ((count1ms % 1000) == 0)
			{
				//flagZeroDetectEr1s = true;
				flag1s = true;
				flcd1s = true;
			}
			/*if ( (count100us % StepRamp) == 0 )
			{
				flagCountRamp = true;
				CountRamp++;
		}*/
			if (count1ms == 10000)
			{
				//fErr2s = true;
				count1ms = 0;
			}
		}
		/*volatile static uint16_t count10ms = 0;
		count10ms++;

		flag10ms = true;
		flcd10ms = true;
		flagADC10ms= true;
		if ( (count10ms % 10) == 0 )
		{
			flag100ms = true;
			flcd100ms = true;
			f2lcd100ms = true;
			fSlope100ms = true;

			if ( (count10ms % 30) == 0 )
			{
				flag300ms = true;
				flcd300ms = true;
		}
		if ( (count10ms % 50) == 0 )
		{
			fLcdError500ms = true;
			flagRefreshValue500ms = true;
			flcd500ms = true;

		}
		if ( (count10ms % 100) == 0 )
		{
			//flagZeroDetectEr1s = true;
			flag1s = true;
			flcd1s = true;
			fupdate1s = true;
		}
		if ( (count10us % StepRamp) == 0 )
		{
			flagCountRamp = true;
			CountRamp++;
		}
		if (count10ms == 1000)
		{
			//fErr2s = true;
			count10ms = 0;
		}
	}*/
	}
	/*if (htim->Instance == TIM3 && flagUpdatePWM == true) {
		if (menu.StateVar[cursorLine].IndexMenu == Second_Menu_1)
		{
			SetFrequency(Frequency); //VA_FREQUENCY
			SetDutyCycle(Time_ON); //VA_TIME
		}
		flagSetPWM = true;
		flagUpdatePWM = false;
	}*/
	/* USER CODE END Callback 1 */
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
