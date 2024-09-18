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
#define BUZZ_NUM	0
#define BUZZ_DELAY	1
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

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM3_Init(void);
static void MX_TIM7_Init(void);
static void MX_I2C1_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
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

static inline void ToggleBuzzer(volatile uint8_t buzzer[2])
{
	static uint8_t number = 0;
	static uint8_t count10ms = 0;

	if (count10ms == 0) /* buzzer[BUZZ_DELAY]/10 = number of count10ms */
	{
		if (number < buzzer[BUZZ_NUM] * 2)	/* buzzer[BUZZ_NUM]*2 = number of (buzzer_off + buzzer_on) BUT
											   buzzer[BUZZ_NUM]	  = number of buzzer_on */
		{
			//BUZZ_TGL;
			HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_10);
			number++;
			count10ms = (buzzer[BUZZ_DELAY] / 10);
		}
		else
		{
			HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, GPIO_PIN_RESET);
			number = 0;
			count10ms = 0;
			buzzer[BUZZ_NUM] = 0;
			buzzer[BUZZ_DELAY] = 0;
		}
	}

	if (buzzer[BUZZ_NUM])
	{
		count10ms--;
	}
}
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM1_Init();
  MX_TIM3_Init();
  MX_TIM7_Init();
  MX_I2C1_Init();
  /* USER CODE BEGIN 2 */
	//menu.InitValueWriteToEeprom();
	/*InitValueReadFromEeprom();
	
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
	pStateFunc = StandBy;
	
	FirstMenuState();
	
    Init();
	Clear();
	DisplayOn();
	BackLightSet();*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_11);
	  HAL_Delay(500);
		  
	  /*encoder_counter = __HAL_TIM_GET_COUNTER(&htim1) / 4;
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
		  Debounce(); // Update button_state.

		 // if (buzzer[BUZZ_NUM])
		  	//ToggleBuzzer(buzzer);

		  flag10ms = false;
	  }

	  if (flag1ms == true && flagSetPWM == true)
	  {
		  CountRamp++;
		  if (CountRamp > StepRamp)
		  {
			  CountRamp = 0;
			  flagSetPWM = false;
			  //Set_DutyCycle(DutyCycle);
		  }
		  else
			  SetDutyCycle((Time_ON / StepRamp) * CountRamp);

		  flag1ms = false;
	  }

	  // Read input and change states
	  buttonMask = KEY_MASK;
	  buttonMask &= Release;
	  Release ^= buttonMask;
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
	  if (errorVar != 0)
		  nextState = ST_ERRORS;
	  if (nextState != state)
	  {
		  state = nextState;

		  for (uint8_t n = 0; (j = StateMenu[n].state); n++)
		  {
			  if (j == state)
			  {
				  pStateFunc = StateMenu[n].pFunc;
				  break;
			  }
		  }

	  }*/
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_1); /* Flash Latency config  */  
	
	SET_BIT(RCC->CR, RCC_CR_HSEON); /* HSE ON  */ 
	while (!(READ_BIT(RCC->CR, RCC_CR_HSERDY) == (RCC_CR_HSERDY))) ; /* Wait for HSE ready  */ 
	
	
	
	//	SET_BIT(RCC->CSR, RCC_CSR_LSION); /* LSI ON  */
	//	while (!(READ_BIT(RCC->CSR, RCC_CSR_LSIRDY) == (RCC_CSR_LSIRDY))) ;/* Wait for LSI ready  */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
	//	SET_BIT(PWR->CR, PWR_CR_DBP); 
	//    SET_BIT(RCC->BDCR, RCC_BDCR_LSEON); /* LSE ON  */
	//	while (!(READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == (RCC_BDCR_LSERDY))) ;/* Wait for LSE ready  */
	CLEAR_BIT(PWR->CR, PWR_CR_DBP); 
	/* PLL Config  */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SWS_HSI); /* System clock source change for RCC_CFGR_PLLSRC Config  */
	CLEAR_BIT(RCC->CR, RCC_CR_PLLON); /* PLL Disable  */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL, ((RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR2_PREDIV_DIV1) & RCC_CFGR_PLLSRC) | RCC_CFGR_PLLMUL9);
	MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PREDIV, ((RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR2_PREDIV_DIV1) & RCC_CFGR2_PREDIV));

	SET_BIT(RCC->CR, RCC_CR_PLLON); /* PLL Enable  */
	
	while (!(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == (RCC_CR_PLLRDY))) ;/* Wait for PLL ready  */
	
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL); /* System clock source PLL  */
	while (!(READ_BIT(RCC->CFGR, RCC_CFGR_SWS))) ;/* Wait for System clock switch status ready  */
	
	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);
	
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE1_DIV1);	
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
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, LCD_BKL_Pin|LCD4_Pin|LCD5_Pin|LCD6_Pin
                          |LCD7_Pin|LED1_Pin|LED2_Pin|LED3_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_EN_Pin|LCD_RS_Pin|Madule_LED_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_BKL_Pin LED1_Pin LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LCD_BKL_Pin|LED1_Pin|LED2_Pin|LED3_Pin;
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
  GPIO_InitStruct.Pin = LCD4_Pin|LCD5_Pin|LCD6_Pin|LCD7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pins : LCD_EN_Pin LCD_RS_Pin */
  GPIO_InitStruct.Pin = LCD_EN_Pin|LCD_RS_Pin;
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
  GPIO_InitStruct.Pin = KeyRotaryMiddle_Pin|KeyStandby_Pin|KeyBack_Pin;
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
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */

	/* USER CODE END Callback 0 */

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
	}
	if (htim->Instance == TIM3 && flagUpdatePWM == true) {
		if (StateVar[cursorLine].IndexMenu == Second_Menu_1)
		{
			SetFrequency(Frequency); //VA_FREQUENCY
			SetDutyCycle(Time_ON); //VA_TIME
		}
		flagSetPWM = true;
		flagUpdatePWM = false;
	}
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
