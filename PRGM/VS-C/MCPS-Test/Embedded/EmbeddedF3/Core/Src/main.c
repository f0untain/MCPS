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
#include "LCD.h"
#include "button.h"
#include "Menu.h"
#include "eeprom.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define LED1_SET(x) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_10, x)
#define LED2_SET(x) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_11, x)
#define LED3_SET(x) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_12, x)
#define LED4_SET(x) HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, x)
#define BUZZ_SET(x) HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, x)
#define LCD_BL_SET(x) HAL_GPIO_WritePin(GPIOC, GPIO_PIN_13, x)

#define BUZZ_NUM	0
#define BUZZ_DELAY	1

#define Freq_CLK    72000000          //72 MHz
#define PWM_PSC     7200-1            //7200
#define PWM_CLK     Freq_CLK/(PWM_PSC+1)  //72 KHz

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
volatile uint8_t /* buttons_release, */ buttonMask;
extern  volatile uint8_t buttons_release;

/* holds the current variable, according to "menu.h" */
uint8_t variable;
volatile uint8_t nextState;// volatile for avoid of optimizing for debug program

// uint8_t currentVariable;// holds the current variable number in order to table, according to "menu.h"
uint8_t cursorLine;		// holds the cursor line, according to "menu.h"

const char* statetext[4];	     // Holds 4 const char pointer to display in lines of LCD.
uint8_t elementType[4];	         // Holds 4 types for values of each variable that display in lines of LCD.
ValueStruct *pValues[4];         // Holds 4 ValueStruct pointer that point to a value for each variable.

uint8_t reWarning = 1;  //for exhibition of warnings and DisplayRunMenu.
uint8_t refreshLcd;		//for refreshing LCD.
uint16_t errorVar = 0x00, warningVar = 0x00;		// Each bit of this variable corresponds to an error state.
uint8_t  scnd,minute,updateTime;  //Update Vmax every few minutes
uint8_t tapChangerState = 0; // Holds output of TapChanger() that represents if TapChanger() completed his job or no.
uint8_t lcdBuffer[20];
uint8_t showARMWELD=true;  // ARMWELD animation in standBy.
static char count1=0;
/* Timing flags.*/
volatile bool	flag1s,
				fupdate1s,
				flag10us,
				flag100us,
				flag1ms,
				flag10ms,
				flag100ms,
				flagRefreshValue500ms,
				flag300ms,
				flagADC10ms,
				flcd1s,
				fSlope100ms,
				flcd10ms,
				flcd100ms,
				f2lcd100ms,
				fLcdError500ms,
				flcd500ms,
				flcd300ms,
				flagUpdatePWM,
				flagSetPWM,
				flagCountRamp,
				flagRotaryCW=false,
				flagRotaryCCW=false;

//rotary CW/CCW diagnose variables
int32_t encoder_counter = 0;
int32_t last_count = 0;
int32_t diff = 0;

float VariableRamp = 0.1;
float StepRamp = 0.0;
uint16_t CountRamp = 0;
float Time_ON;
float Frequency;

static char enterFunction = 1;

/* Array to determine that the buzzer need to toggle.
 * this array consists of buzzer[0] = number of toggles
 *					  and buzzer[1] = delay between each toggle.
 * this array must be initialized when need to buzzer toggled.
 */
volatile uint8_t buzzer[2];

//const STATE_NEXTVARIABLE *pStateNextVar[4];
//const STATE_VARIABLE *pStateVar[4];
//const MENU_STATE *pStateMenu[4];

STATE_NEXTVARIABLE StateNextVar[15];
STATE_VARIABLE StateVar[8];
MENU_STATE StateMenu[5];

/* variables for store compare value of TCC1(the PWM timer).*/
uint16_t compareValue;

/* Strings that will show in value column in LCD.*/
const char NON[]	= "None";
const char LOW[]	= "LOW ";
const char MED[] 	= "MED ";
const char HIGH[]   = "HIGH";

/* in EEPROM for save recent value before shut down.*/
ValueStruct /*EEMEM*/ EEValues_Alfa[ NUMBER_OF_VARIABLES+1 ] = {
//	VARIABLE_VALUE						STRING_NUMBER				DOWN_LIMIT					  UP_LIMIT
	{.pText = { 0, 0},					.strNum = 0,				.downLimit = 0,				  .upLimit = 0},                /* This line is added for correction of difference between variable numbers(one based) and values arrays(zero based).*/
	{.pText = {NON, LOW, MED, HIGH},	.strNum = VOL_LEVEL_LOW,    .downLimit = VOL_LEVEL_NONE,  .upLimit = VOL_LEVEL_HIGH},	/* Voltage Value*/
	{.fval = 10.0f,												    .downLimit = 0.5f,	          .upLimit = 30.9f},		    /* Frq*/
	{.fval = 2.0f,													.downLimit = 0.1f,			  .upLimit = 4.9f},			    /* time*/
	{.fval = 5.0,				                                    .downLimit = 0.5f,	          .upLimit = 4.9f},		        /* Ramp*/
	{.ival = 10,													.downLimit = 1,	 			  .upLimit = 20},			    /* Rythm on*/
	{.ival = 10,			  	                                    .downLimit = 1,	              .upLimit = 20},	     	    /* Rythm off*/
};
ValueStruct /*EEMEM*/ EEValues_Beta[ NUMBER_OF_VARIABLES+1 ] = {
//	VARIABLE_VALUE						STRING_NUMBER				DOWN_LIMIT					  UP_LIMIT
	{.pText = { 0, 0},					.strNum = 0,				.downLimit = 0,				  .upLimit = 0},                /* This line is added for correction of difference between variable numbers(one based) and values arrays(zero based).*/
	{.pText = {NON, LOW, MED, HIGH},	.strNum = VOL_LEVEL_LOW,    .downLimit = VOL_LEVEL_NONE,  .upLimit = VOL_LEVEL_HIGH},	/* Voltage Value*/
	{.fval = 10.0f,												    .downLimit = 0.1f,			  .upLimit = 30.9f},		    /* Frq*/
	{.fval = 5.0f,													.downLimit = 0.1f,			  .upLimit = 20.9f},			/* time*/
	{.ival = 2,				                                        .downLimit = 1,	              .upLimit = 10},		        /* Ramp*/
	{.ival = 10,													.downLimit = 1,	 			  .upLimit = 20},			    /* Rythm on*/
	{.ival = 10,			  	                                    .downLimit = 1,	              .upLimit = 20},	     	    /* Rythm off*/
};
ValueStruct /*EEMEM*/ EEValues_Teta[ NUMBER_OF_VARIABLES+1 ] = {
//	VARIABLE_VALUE						STRING_NUMBER				DOWN_LIMIT					  UP_LIMIT
	{.pText = { 0, 0},					.strNum = 0,				.downLimit = 0,				  .upLimit = 0},                /* This line is added for correction of difference between variable numbers(one based) and values arrays(zero based).*/
	{.pText = {NON, LOW, MED, HIGH},	.strNum = VOL_LEVEL_LOW,    .downLimit = VOL_LEVEL_NONE,  .upLimit = VOL_LEVEL_HIGH},	/* Voltage Value*/
	{.fval = 10.0f,												    .downLimit = 0.1f,			  .upLimit = 30.9f},		    /* Frq*/
	{.fval = 5.0f,													.downLimit = 0.1f,			  .upLimit = 20.9f},			/* time*/
	{.ival = 3,				                                        .downLimit = 1,	              .upLimit = 10},		        /* Ramp*/
	{.ival = 10,													.downLimit = 1,	 			  .upLimit = 20},			    /* Rythm on*/
	{.ival = 10,			  	                                    .downLimit = 1,	              .upLimit = 20},	     	    /* Rythm off*/
};
ValueStruct /*EEMEM*/ EEValues_Delta[ NUMBER_OF_VARIABLES+1 ] = {
//	VARIABLE_VALUE						STRING_NUMBER				DOWN_LIMIT					  UP_LIMIT
	{.pText = { 0, 0},					.strNum = 0,				.downLimit = 0,				  .upLimit = 0},                /* This line is added for correction of difference between variable numbers(one based) and values arrays(zero based).*/
	{.pText = {NON, LOW, MED, HIGH},	.strNum = VOL_LEVEL_LOW,    .downLimit = VOL_LEVEL_NONE,  .upLimit = VOL_LEVEL_HIGH},	/* Voltage Value*/
	{.fval = 10.0f,												    .downLimit = 0.1f,			  .upLimit = 30.9f},		    /* Frq*/
	{.fval = 5.0f,													.downLimit = 0.1f,			  .upLimit = 20.9f},			/* time*/
	{.ival = 4,				                                        .downLimit = 1,	              .upLimit = 10},		        /* Ramp*/
	{.ival = 10,													.downLimit = 1,	 			  .upLimit = 20},			    /* Rythm on*/
	{.ival = 10,			  	                                    .downLimit = 1,	              .upLimit = 20},	     	    /* Rythm off*/
};
ValueStruct /*EEMEM*/ EEValues_Gama[ NUMBER_OF_VARIABLES+1 ] = {
//	VARIABLE_VALUE						STRING_NUMBER				DOWN_LIMIT					  UP_LIMIT
	{.pText = { 0, 0},					.strNum = 0,				.downLimit = 0,				  .upLimit = 0},                /* This line is added for correction of difference between variable numbers(one based) and values arrays(zero based).*/
	{.pText = {NON, LOW, MED, HIGH},	.strNum = VOL_LEVEL_LOW,    .downLimit = VOL_LEVEL_NONE,  .upLimit = VOL_LEVEL_HIGH},	/* Voltage Value*/
	{.fval = 10.0f,												    .downLimit = 0.1f,			  .upLimit = 30.9f},		    /* Frq*/
	{.fval = 5.0f,													.downLimit = 0.1f,			  .upLimit = 20.9f},			/* time*/
	{.ival = 5,				                                        .downLimit = 1,	              .upLimit = 10},		        /* Ramp*/
	{.ival = 10,													.downLimit = 1,	 			  .upLimit = 20},			    /* Rythm on*/
	{.ival = 10,			  	                                    .downLimit = 1,	              .upLimit = 20},	     	    /* Rythm off*/
};
ValueStruct /*EEMEM*/ EEValues_Custom[ NUMBER_OF_VARIABLES+1 ] = {
//	VARIABLE_VALUE						STRING_NUMBER				DOWN_LIMIT					  UP_LIMIT
	{.pText = { 0, 0},					.strNum = 0,				.downLimit = 0,				  .upLimit = 0},                /* This line is added for correction of difference between variable numbers(one based) and values arrays(zero based).*/
	{.pText = {NON, LOW, MED, HIGH},	.strNum = VOL_LEVEL_LOW,    .downLimit = VOL_LEVEL_NONE,  .upLimit = VOL_LEVEL_HIGH},	/* Voltage Value*/
	{.fval = 10.0f,												    .downLimit = 0.1f,			  .upLimit = 30.9f},		    /* Frq*/
	{.fval = 5.0f,													.downLimit = 0.1f,			  .upLimit = 20.9f},			/* time*/
	{.ival = 6,				                                        .downLimit = 1,	              .upLimit = 10},		        /* Ramp*/
	{.ival = 10,													.downLimit = 1,	 			  .upLimit = 20},			    /* Rythm on*/
	{.ival = 10,			  	                                    .downLimit = 1,	              .upLimit = 20},	     	    /* Rythm off*/
};

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
char ContactUs( char input )
{
	static uint8_t enterFunction = 1;
	if ( enterFunction==1 )
	{
		LCD_Clear();
		LCD_Puts(1,LCD_LINE1, "Tel:021-66760674");
		LCD_Puts(1,LCD_LINE2, "Tel:09191143937");
		LCD_Puts(1,LCD_LINE3, "website");
		LCD_Puts(2,LCD_LINE4, "Nekoorad.com");
	}
	enterFunction=0;
	if (input == KEY_STANDBY)
	{
		enterFunction = 1;
		return ST_STANDBY;
	}
	else if (input==KEY_BACK)
	{
		refreshLcd = true;  // atention to DisplyRunMenu	function
		variable = ST_CHANGE_MENU;
		enterFunction = 1;
		return ST_MAIN_STATE;
	}

 	return ST_CONTACT_US;
}

static inline void ToggleBuzzer(volatile uint8_t buzzer[2])
{
	static uint8_t number = 0;
	static uint8_t count10ms = 0;

	if ( count10ms == 0 ) /* buzzer[BUZZ_DELAY]/10 = number of count10ms */
	{
		if ( number < buzzer[BUZZ_NUM]*2)	/* buzzer[BUZZ_NUM]*2 = number of (buzzer_off + buzzer_on) BUT
											   buzzer[BUZZ_NUM]	  = number of buzzer_on */
		{
			//BUZZ_TGL;
			HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_10);
			number++;
			count10ms = (buzzer[BUZZ_DELAY]/10);
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

// It draws lines in lcd.
void HLine (char x, char y, uint8_t KindofLine,char mem8)
{
	 LCD_CreateChar(mem8,line[KindofLine]);
	 LCD_CursorSet(x,y);
	 LCD_Data(mem8);
}

// It shows weld animation.
void WeldDisplay(void)
{
	 uint8_t ArcNumber=0;
	 while (ArcNumber<5)
	 {
		 if (flcd100ms)
		 {
			 LCD_CreateChar(LCD_Mem7,arcSymbol[ArcNumber]);
			 LCD_CursorSet(10,LCD_LINE1);
			 LCD_Data(LCD_Mem7);
			 ArcNumber++;
			 flcd100ms=false;
		 }

	 }
}

/*! \brief This function displays LCD texts.
 *
 *	This function displays LCD texts each 500 mS or if when a change in data occurs.
 *
 */
static inline void DisplayRunMenu(void)
{
	char lcdBuffer[12];

	if ( flagRefreshValue500ms == true )
	{
		for ( uint8_t n = 0 ; n < 4 ; n++)
		{
			/* Display values */
			switch(elementType[n])
			{
				case ET_FLT:	sprintf(lcdBuffer, "%-4.1f", pValues[n]->fval);
								LCD_Puts(14,n, lcdBuffer);
								break;

				case ET_INT:
					            sprintf(lcdBuffer, "%-3d", pValues[n]->ival);
								LCD_Puts(14,n, lcdBuffer);
								break;

				case ET_FSTR:
					            LCD_Puts(14,n, pValues[n]->pText[pValues[n]->strNum]);
								break;


				/*case ET_ULNG:	sprintf(lcdBuffer, "%lu", pValues[n]->ulval);
								LCD_Puts(14,n, lcdBuffer);
								break;*/

			}
			flagRefreshValue500ms = false;
		}
	}

	// Plain menu text
	if (refreshLcd == true)
	{
		LCD_Clear();
		LCD_CursorSet(0,cursorLine);
		LCD_Data(0x7E);  //  "->" character

		for ( uint8_t n = 0 ; n < 4 ; n++)
		{
			// refresh lcd.
			LCD_Puts(1,n, statetext[n]);

			/* Display values */
			switch(elementType[n])
			{
				case ET_FLT:	sprintf(lcdBuffer, "%-4.1f", pValues[n]->fval);
								LCD_Puts(14,n, lcdBuffer);
								break;

				case ET_INT:	sprintf(lcdBuffer, "%-3d", pValues[n]->ival);
								LCD_Puts(14,n, lcdBuffer);
								break;

				case ET_FSTR:	LCD_Puts(14,n, pValues[n]->pText[pValues[n]->strNum]);
								break;

				/*case ET_ULNG:	sprintf(lcdBuffer, "%lu", pValues[n]->ulval);
				                LCD_Puts(14,n, lcdBuffer);
								break;*/

			}
		}

		refreshLcd = false;
	}
}

 /*! \brief This function shifts between the different variables.
  *
  * \param state			Current state.
  * \param stimuli		Button input.
  * \param cursorLine	Pointer for save next cursor line to it.
  *
  * \return nextVariable    next state according to the current state and button input.
  */
unsigned char VariableMachine(unsigned char stimuli)
 {
 	unsigned char nextVariable = variable;    // Default stay in same state
 	unsigned char i, j;
 	uint8_t cursorBuffer;

 	for (i=0; ( j=StateNextVar[i].variable) ; i++ )
 	{
 		if ( j == variable &&
 				StateNextVar[i].input== stimuli)

 			{
 			nextVariable = StateNextVar[i].nextVariable;

 			cursorBuffer = StateNextVar[i].cursorLine;


 			if ( cursorBuffer != LCD_LINE_RECENT )
 			cursorLine = cursorBuffer;


 			break;
 		}
 	}


 	return nextVariable;
 }

char StandBy(char input)
{
	static char x1_dot=0/*,count1=0*/;

	static uint8_t	enterFunction = 1;

	if ( enterFunction==1 )
	{
		buzzer[BUZZ_NUM] = 1;
		buzzer[BUZZ_DELAY] = 255;
		enterFunction =0;
	}


	if(reWarning == 0 || warningVar == 0)	//need for show after some second warning occured or not any warnings.
	{
			if(f2lcd100ms)
			{

				// display animation of medinalTeb first time when turn on after that show "STANDBY"
				if (x1_dot>=6 && count1<19)
				{
					count1++;
					if(count1<=5)
					{
						HLine(9+count1,LCD_LINE3,LCD_UpLine,LCD_Mem0);
						HLine(10-count1,LCD_LINE3,LCD_UpLine,LCD_Mem0);
					}
					else if (count1==6)
					{
						HLine(9+count1,LCD_LINE2,LCD_RightLine,LCD_Mem1);
						HLine(10-count1,LCD_LINE2,LCD_LeftLine,LCD_Mem2);
					}
					else if (count1<=11)
					{
						HLine(21-count1,LCD_LINE1,LCD_DownLine,LCD_Mem3);
						HLine(count1-2,LCD_LINE1,LCD_DownLine,LCD_Mem3);
					}
					else if (count1<=12)
					WeldDisplay();

				}
				else if (count1==19)
				{
					LCD_Clear();
					LCD_Puts(7,LCD_LINE2, "STANDBY");
					showARMWELD=false;
//					WELD_FAN_OFF;   // turn off fan when ARMWELD animation finish.
					count1++;  // it runs just first time this (if).
				}
				f2lcd100ms=false;
			}

			if (flcd500ms )
			{
				if (showARMWELD==true)
				{
//					WELD_FAN_ON;   // turn on fan when the device  is turning on.
					if(x1_dot<6)
					{
						LCD_Clear();
						x1_dot++;
					}
					LCD_Puts(x1_dot,1, "NEKOO");
					LCD_Puts((17-x1_dot),1, "RAD");
				}

				flcd500ms= false;
			}

	}
 	if ( input == KEY_STANDBY )
	{
		enterFunction = 1;
		showARMWELD=false;  // because of don't show ARMWELD when come from RunMeno.
 		return ST_MAIN_STATE;
	}

 	return ST_STANDBY;
}

char MainMenu(char input)
{
	uint8_t j;
	static uint8_t nextVariable = VA_FIRST_VARIABLE;
	static uint8_t cursor_variable;
	static uint8_t adjustState = ST_MAIN_STATE;
	static uint8_t cursor_main=0;
	//static char enterFunction = 1;

	/*if (values[VA_WAVE_TYPE].strNum == VOL_LEVEL_NONE )
	{

	}*/

	if ( nextVariable != variable )
	{
		variable = nextVariable;
		refreshLcd = true;  // atention to DisplyRunMenu	function
		for ( uint8_t currentVariable = 0; (j=StateVar[currentVariable].variable); currentVariable++ )
		{
			/* macro to determine which variable must be displayed in first line in LCD. */
			#define FIRST_LINE_VAR		currentVariable-cursorLine

			if ( j == variable )
			{
				adjustState = StateVar[currentVariable].adjustState;
				for ( uint8_t n = 0 ; n<4 ; n++ )
				{
					statetext[n]  = StateVar[FIRST_LINE_VAR +n].pText;
					elementType[n] = StateVar[FIRST_LINE_VAR +n].elementType;
					pValues[n] = (ValueStruct*)StateVar[FIRST_LINE_VAR +n].pValueStruct;
				}
				break;
			}
		}
		if( reWarning == 0 || warningVar == 0 )	//need for show after some second warning occured or not any warnings.
			DisplayRunMenu();
	}
	if ( input == KEY_STANDBY && StateVar[cursorLine].IndexMenu == First_Menu )
	{
		count1=19;
		nextVariable = VA_FIRST_VARIABLE;
		cursorLine = LCD_LINE1;
		variable=VA_SECOND_VARIABLE;
		return ST_STANDBY;
	}
	else if ( input == KEY_ACCEPT )
	{
		if( StateVar[cursorLine].IndexMenu == First_Menu )
		{
			if( adjustState == ST_CONTACT_US )
				return adjustState;
			else
			{
				if( variable==MAIN_Alfa )
				{
					memcpy(&StateVar, &stateVariable_Alfa, sizeof(stateVariable_Alfa));
					Frequency = values_Alfa[2].fval;
					Time_ON = values_Alfa[3].fval;
					StepRamp = (values_Alfa[4].fval * 1000)/1.000;
				}
				else if( variable==MAIN_Beta )
					memcpy(&StateVar, &stateVariable_Beta, sizeof(stateVariable_Beta));
				else if( variable==MAIN_Teta )
					memcpy(&StateVar, &stateVariable_Teta, sizeof(stateVariable_Teta));
				else if( variable==MAIN_Delta )
					memcpy(&StateVar, &stateVariable_Delta, sizeof(stateVariable_Delta));
				else if( variable==MAIN_Gama )
					memcpy(&StateVar, &stateVariable_Gama, sizeof(stateVariable_Gama));
				else if( variable==MAIN_Custom )
					memcpy(&StateVar, &stateVariable_Custom, sizeof(stateVariable_Custom));

				memcpy(&StateNextVar, &state_nextVariable, sizeof(state_nextVariable));
				//memcpy(&StateVar, &stateVariable, sizeof(stateVariable));
				memcpy(&StateMenu, &menuState, sizeof(menuState));
				nextVariable = VA_EIGHTH_VARIABLE;
				cursor_main = cursorLine;
				cursorLine = LCD_LINE1;
				cursor_variable = variable;
				return (adjustState);
			}
		}
		else
		{
			return adjustState;
		}
	}
	else if ( input == KEY_BACK )
	{
		memcpy(&StateNextVar, &main_state_nextVariable, sizeof(main_state_nextVariable));
		memcpy(&StateVar, &main_stateVariable, sizeof(main_stateVariable));
		memcpy(&StateMenu, &main_menuState, sizeof(main_menuState));
		cursorLine = cursor_main;
		//nextVariable = cursorLine+1;
	    nextVariable = cursor_variable;
		return ST_MAIN_STATE;
	}
	else if ( input == KEY_START )
	{
		if( StateVar[cursorLine].IndexMenu != First_Menu )
		{
			// Act like Start Buttom
			if ( enterFunction == 1 )  // If it is the first time the button is pushed and the variables of frequency and time-on(duty cycle) are set
			{
				HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
				LED1_SET(1);
				flagUpdatePWM = true; // set variable of frequency and duty cycle of PWM on TIM3 Intrupt
				enterFunction = 0;
			}
			//Act like Stop Buttom
			else
			{
				HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
				LED1_SET(0);
				enterFunction = 1;
			}
		}
		return ST_MAIN_STATE;
	}
	else if ( input != KEY_NULL )
	{
		// Plain menu, clock the state machine
		nextVariable = VariableMachine(input);
	}

	return ST_MAIN_STATE;
}

/*! \brief This function adjusts integer values.
 *
 * \param input			Buttons state after debouncig.
 *
 * \return nextState    next state according to the current state and button input.
 */
char IntAdjust(char input)
{
	int16_t valueIA;
    uint8_t j=1;
	static char enterFunction = 1;

	char lcdBufferIA[15];

	if ( enterFunction==1 )
	{
		//pValues[cursorLine]->ival=values[cursorLine+1].ival;
		TIM1->CNT = (pValues[cursorLine]->ival)*ROTARY_SPC;       //The amount of variable(that cursor point it)  return to CNT of timer or QDEC .
		enterFunction = 0;
	}
	LCD_CreateChar(0,updownCharacter);// "<>" character
	LCD_CursorSet(13,cursorLine);
	LCD_Data(0);  //  "<>" character


	/* Check timer valueIA to be in range. */
	valueIA = (int16_t)TIM1->CNT/ROTARY_SPC;// int16_t to avoid of roll over timer valueIA from bottom to top.

	if( valueIA > (int16_t)pValues[cursorLine]->upLimit )
		TIM1->CNT = (int16_t)pValues[cursorLine]->upLimit*ROTARY_SPC;

	else if ( valueIA < (int16_t)pValues[cursorLine]->downLimit )
		TIM1->CNT = (int16_t)pValues[cursorLine]->downLimit*ROTARY_SPC;

	sprintf(lcdBufferIA,"%-3d", TIM1->CNT/ROTARY_SPC);
	LCD_Puts(14,cursorLine, lcdBufferIA);

	switch (input)
	{
		case KEY_ACCEPT:
			pValues[cursorLine]->ival = TIM1->CNT/ROTARY_SPC;

			if( StateVar[cursorLine].IndexMenu == Second_Menu_1 ) //if we are in sub menu Alfa:
			{
				for(uint8_t i=1; i<=NUMBER_OF_VARIABLES; i++)
				{
					EEPROM_Write(i,0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
				}
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_2)
			{
				for(uint8_t i=(1+(1*NUMBER_OF_VARIABLES)); i<=(2*NUMBER_OF_VARIABLES); i++)
				{
				    EEPROM_Write(i,0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
				    j++;
					if(j==7)
						j=1;
				}
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_3)
			{
				for(uint8_t i=(1+(2*NUMBER_OF_VARIABLES)); i<=(3*NUMBER_OF_VARIABLES); i++)
				{
					EEPROM_Write(i,0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
					j++;
					if(j==7)
						j=1;
				}
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_4)
			{
				for(uint8_t i=(1+(3*NUMBER_OF_VARIABLES)); i<=(4*NUMBER_OF_VARIABLES); i++)
				{
					EEPROM_Write(i,0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
					j++;
					if(j==7)
						j=1;
				}
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_5)
			{
				for(uint8_t i=(1+(4*NUMBER_OF_VARIABLES)); i<=(5*NUMBER_OF_VARIABLES); i++)
				{
					EEPROM_Write(i,0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
					j++;
					if(j==7)
						j=1;
				}
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_6)
			{
				for(uint8_t i=(1+(5*NUMBER_OF_VARIABLES)); i<=(6*NUMBER_OF_VARIABLES); i++)
				{
					EEPROM_Write(i,0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
					j++;
					if(j==7)
						j=1;
				}
			}
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_MAIN_STATE;

		/*case KEY_BACK:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_MAIN_STATE;*/

		case KEY_STANDBY:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_STANDBY;
	}
	return ST_INT_ADJUST;
}

/*! \brief This function adjusts string values.
 *
 * \param input			Buttons state after debouncig.
 *
 * \return nextState    next state according to the current state and button input.
 */
char StringChange(char input)
{
	static char enterFunction = 1;
	static int8_t strNumTemp;
	if ( enterFunction==1 )
	{
		strNumTemp = pValues[cursorLine]->strNum;
		enterFunction = 0;
	}

	switch (input)
	{
		case ROTARY_CW_DIR:		strNumTemp ++;	break;

		case ROTARY_CCW_DIR:	strNumTemp --;	break;

		case KEY_ACCEPT:

			// store the temporary adjusted value to the global variable
			pValues[cursorLine]->strNum = strNumTemp;

			if(StateVar[cursorLine].IndexMenu==Second_Menu_1)
			{
				if ( variable == VA_VOLTAGE /*&& pValues[cursorLine]->strNum==VOL_LEVEL_LOW*/ )
				{
					//LED2_SET(1);
				}
				EEPROM_Write(1,0, (uint8_t*)&values_Alfa[1], sizeof(ValueStruct));
			}

			else if(StateVar[cursorLine].IndexMenu==Second_Menu_2)
			{
				EEPROM_Write((1+(1*NUMBER_OF_VARIABLES)),0, (uint8_t*)&values_Beta[1], sizeof(ValueStruct));
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_3)
			{
                EEPROM_Write((1+(2*NUMBER_OF_VARIABLES)),0, (uint8_t*)&values_Teta[1], sizeof(ValueStruct));
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_4)
			{
				EEPROM_Write((1+(3*NUMBER_OF_VARIABLES)),0, (uint8_t*)&values_Delta[1], sizeof(ValueStruct));
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_5)
			{
				EEPROM_Write((1+(4*NUMBER_OF_VARIABLES)),0, (uint8_t*)&values_Gama[1], sizeof(ValueStruct));
			}
			else if(StateVar[cursorLine].IndexMenu==Second_Menu_6)
			{
				EEPROM_Write((1+(5*NUMBER_OF_VARIABLES)),0, (uint8_t*)&values_Custom[1], sizeof(ValueStruct));
			}
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character

			return ST_MAIN_STATE;

		/*case KEY_BACK:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_MAIN_STATE;*/

		case KEY_STANDBY:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_STANDBY;
	}

	LCD_CreateChar(0,updownCharacter);// "<>" character
	LCD_CursorSet(13,cursorLine);
	LCD_Data(0);  //  "<>" character


	if( strNumTemp > pValues[cursorLine]->upLimit )
	{
		strNumTemp = pValues[cursorLine]->upLimit;
	}
	else if ( strNumTemp < pValues[cursorLine]->downLimit )
	{
		strNumTemp = pValues[cursorLine]->downLimit;
	}
	LCD_Puts(14,cursorLine,pValues[cursorLine]->pText[strNumTemp]);

	return ST_STR_CHANGE;
}

/*! \brief This function adjusts float values.
 *
 * \param input			Buttons state after debouncig.
 *
 * \return nextState    next state according to the current state and button input.
 */
char FloatAdjust(char input)
{
    #define INTEGER_PART	0
    #define DECIMAL_PART	1
	static uint8_t mode = INTEGER_PART;
	static int FloatTemp[2];
	static char enterFunction = 1;
	uint8_t j=1;
	char lcdBufferIA[15];
    //int16_t UpLimit_DEC;
	int16_t DownLimit_DEC;

	if ( enterFunction==1 )
	{
		//FloatTemp[DECIMAL_PART] = modf(pValues[cursorLine]->fval, &FloatTemp[INTEGER_PART]);
		FloatTemp[INTEGER_PART] = (int) pValues[cursorLine]->fval;
		FloatTemp[DECIMAL_PART] = round((pValues[cursorLine]->fval - FloatTemp[INTEGER_PART])*10);

		//UpLimit_DEC = round((pValues[cursorLine]->upLimit - (int)pValues[cursorLine]->upLimit)*10);
		DownLimit_DEC = round((pValues[cursorLine]->downLimit - (int)pValues[cursorLine]->downLimit)*10);

		if (mode == INTEGER_PART)
		{
			TIM1->CNT = (int16_t)(FloatTemp[INTEGER_PART])*ROTARY_SPC;
		}
		/*else if ( mode == DECIMAL_PART)
		{
			TIM1->CNT = ((int16_t)(FloatTemp[DECIMAL_PART])*10)*ROTARY_SPC;
		}*/
		enterFunction = 0;
	}

	if (mode == INTEGER_PART)
	{
		FloatTemp[INTEGER_PART] = (int16_t)TIM1->CNT/ROTARY_SPC;
		if( FloatTemp[INTEGER_PART] > (int16_t)pValues[cursorLine]->upLimit )
			TIM1->CNT = (int16_t)pValues[cursorLine]->upLimit*ROTARY_SPC;

		else if ( FloatTemp[INTEGER_PART] < (int16_t)pValues[cursorLine]->downLimit )
			TIM1->CNT = (int16_t)pValues[cursorLine]->downLimit*ROTARY_SPC;

		LCD_CreateChar(0,updownCharacter);// "<>" character

		LCD_CursorSet(19,cursorLine);
		LCD_Data(0x20);  //  "blank" character

		LCD_CursorSet(13,cursorLine);
		LCD_Data(0);  //  "<>" character
	}
	else if (mode == DECIMAL_PART)
	{
		FloatTemp[DECIMAL_PART] = (int16_t)TIM1->CNT/ROTARY_SPC;
		if( FloatTemp[DECIMAL_PART] > 9 )
			TIM1->CNT = 9*ROTARY_SPC;

		else if ( FloatTemp[DECIMAL_PART] < 0 )
			TIM1->CNT = 0*ROTARY_SPC;

        //The value of the float number should not be below downlimit of the value
		else if ( FloatTemp[DECIMAL_PART] == 0 && FloatTemp[INTEGER_PART]==0 )
			TIM1->CNT = DownLimit_DEC*ROTARY_SPC;

		LCD_CreateChar(0,updownCharacter);// "<>" character

		LCD_CursorSet(19,cursorLine);
		LCD_Data(0);  //  "<>" character

		LCD_CursorSet(13,cursorLine);
		LCD_Data(0x20);  //  "blank" character
	}

	// store the temporary adjusted value to the global variable
	pValues[cursorLine]->fval = (FloatTemp[INTEGER_PART] + (FloatTemp[DECIMAL_PART]/10.0));
	sprintf(lcdBufferIA,"%-4.1f", pValues[cursorLine]->fval);
	LCD_Puts(14,cursorLine, lcdBufferIA);

	switch (input)
	{
		case ROTARY_CW_DIR:  FloatTemp[mode] ++;   break;

		case ROTARY_CCW_DIR:  FloatTemp[mode] --;	break;

		case KEY_ACCEPT:

			pValues[cursorLine]->fval = (FloatTemp[INTEGER_PART] + (FloatTemp[DECIMAL_PART]/10.0));
			if (mode == INTEGER_PART)
			{
				mode++;
				TIM1->CNT = (int16_t)(FloatTemp[DECIMAL_PART])*ROTARY_SPC;
				break;
			}
			else if (mode == DECIMAL_PART)
			{
				if ( variable == VA_FREQUENCY )
				{
					Frequency = (pValues[cursorLine]->fval)/1.000;
				}
				else if ( variable == VA_TIME )
				{
					Time_ON = (pValues[cursorLine]->fval/1.000);
				}
				else if ( variable == VA_RAMP /*&& pValues[cursorLine]->ival==3*/ )
				{
					StepRamp = (pValues[cursorLine]->fval * 1000)/1.000;
				}

				if ( enterFunction == 0 && variable != VA_RAMP) // change variables in the RUN time
					flagUpdatePWM = true;

				if(StateVar[cursorLine].IndexMenu==Second_Menu_1)
				{
					for(uint8_t i=1; i<=NUMBER_OF_VARIABLES; i++)
					{
						EEPROM_Write(i,0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
					}
				}
				else if(StateVar[cursorLine].IndexMenu==Second_Menu_2)
				{
					for(uint8_t i=(1+(1*NUMBER_OF_VARIABLES)); i<=(2*NUMBER_OF_VARIABLES); i++)
					{
					    EEPROM_Write(i,0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
					    j++;
						if(j==7)
							j=1;
					}
				}
				else if(StateVar[cursorLine].IndexMenu==Second_Menu_3)
				{
					for(uint8_t i=(1+(2*NUMBER_OF_VARIABLES)); i<=(3*NUMBER_OF_VARIABLES); i++)
					{
						EEPROM_Write(i,0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
						j++;
						if(j==7)
							j=1;
					}
				}
				else if(StateVar[cursorLine].IndexMenu==Second_Menu_4)
				{
					for(uint8_t i=(1+(3*NUMBER_OF_VARIABLES)); i<=(4*NUMBER_OF_VARIABLES); i++)
					{
						EEPROM_Write(i,0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
						j++;
						if(j==7)
							j=1;
					}
				}
				else if(StateVar[cursorLine].IndexMenu==Second_Menu_5)
				{
					for(uint8_t i=(1+(4*NUMBER_OF_VARIABLES)); i<=(5*NUMBER_OF_VARIABLES); i++)
					{
						EEPROM_Write(i,0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
						j++;
						if(j==7)
							j=1;
					}
				}
				else if(StateVar[cursorLine].IndexMenu==Second_Menu_6)
				{
					for(uint8_t i=(1+(5*NUMBER_OF_VARIABLES)); i<=(6*NUMBER_OF_VARIABLES); i++)
					{
						EEPROM_Write(i,0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
						j++;
						if(j==7)
							j=1;
					}
				}

				mode = INTEGER_PART;
				enterFunction = 1;
				LCD_CursorSet(19,cursorLine);
				LCD_Data(0x20);  //  "blank" character
				return ST_MAIN_STATE;
			}

		/*case KEY_BACK:
			if (mode == DECIMAL_PART)
			{
				mode=INTEGER_PART;
				TIM1->CNT = (int16_t)(FloatTemp[INTEGER_PART])*ROTARY_SPC;
				break;
			}

			else
			{
				enterFunction = 1;
				LCD_CursorSet(13,cursorLine);
				LCD_Data(0x20);  //  "blank" character
				return ST_MAIN_STATE;
			}*/

		case KEY_STANDBY:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_STANDBY;
	}

	return ST_FLT_ADJUST;
}

//Write initial values  in external eeprom.
void  ValuInit_Write_EE(void)
{
	uint8_t j=1;
	for(uint8_t i=1; i<=NUMBER_OF_VARIABLES; i++)
	{
		EEPROM_Write(i,0, (uint8_t*)&EEValues_Alfa[i], sizeof(ValueStruct));
	}
	for(uint8_t i=(1+(1*NUMBER_OF_VARIABLES)); i<=(2*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Write(i,0, (uint8_t*)&EEValues_Beta[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
	for(uint8_t i=(1+(2*NUMBER_OF_VARIABLES)); i<=(3*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Write(i,0, (uint8_t*)&EEValues_Teta[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
	for(uint8_t i=(1+(3*NUMBER_OF_VARIABLES)); i<=(4*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Write(i,0, (uint8_t*)&EEValues_Delta[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
	for(uint8_t i=(1+(4*NUMBER_OF_VARIABLES)); i<=(5*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Write(i,0, (uint8_t*)&EEValues_Gama[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
	for(uint8_t i=(1+(5*NUMBER_OF_VARIABLES)); i<=(6*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Write(i,0, (uint8_t*)&EEValues_Custom[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
}

//Read initial values from external eeprom.
void ValuInit_Read_EE(void)
{
	uint8_t j=1;
	for(uint8_t i=1; i<=NUMBER_OF_VARIABLES; i++)
	{
		EEPROM_Read(i,0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
	}
	for(uint8_t i=(1+(1*NUMBER_OF_VARIABLES)); i<=(2*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Read(i,0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
	for(uint8_t i=(1+(2*NUMBER_OF_VARIABLES)); i<=(3*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Read(i,0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
	for(uint8_t i=(1+(3*NUMBER_OF_VARIABLES)); i<=(4*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Read(i,0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
	for(uint8_t i=(1+(4*NUMBER_OF_VARIABLES)); i<=(5*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Read(i,0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}
	for(uint8_t i=(1+(5*NUMBER_OF_VARIABLES)); i<=(6*NUMBER_OF_VARIABLES); i++)
	{
		EEPROM_Read(i,0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
		j++;
		if(j==7)
			j=1;
	}

	values_Alfa[1].pText[VOL_LEVEL_NONE] = NON;
	values_Alfa[1].pText[VOL_LEVEL_LOW] = LOW;
	values_Alfa[1].pText[VOL_LEVEL_MED] = MED;
	values_Alfa[1].pText[VOL_LEVEL_HIGH] = HIGH;

	values_Beta[1].pText[VOL_LEVEL_NONE] = NON;
	values_Beta[1].pText[VOL_LEVEL_LOW] = LOW;
	values_Beta[1].pText[VOL_LEVEL_MED] = MED;
	values_Beta[1].pText[VOL_LEVEL_HIGH] = HIGH;

	values_Teta[1].pText[VOL_LEVEL_NONE] = NON;
	values_Teta[1].pText[VOL_LEVEL_LOW] = LOW;
	values_Teta[1].pText[VOL_LEVEL_MED] = MED;
	values_Teta[1].pText[VOL_LEVEL_HIGH] = HIGH;

	values_Delta[1].pText[VOL_LEVEL_NONE] = NON;
	values_Delta[1].pText[VOL_LEVEL_LOW] = LOW;
	values_Delta[1].pText[VOL_LEVEL_MED] = MED;
	values_Delta[1].pText[VOL_LEVEL_HIGH] = HIGH;

	values_Gama[1].pText[VOL_LEVEL_NONE] = NON;
	values_Gama[1].pText[VOL_LEVEL_LOW] = LOW;
	values_Gama[1].pText[VOL_LEVEL_MED] = MED;
	values_Gama[1].pText[VOL_LEVEL_HIGH] = HIGH;

	values_Custom[1].pText[VOL_LEVEL_NONE] = NON;
	values_Custom[1].pText[VOL_LEVEL_LOW] = LOW;
	values_Custom[1].pText[VOL_LEVEL_MED] = MED;
	values_Custom[1].pText[VOL_LEVEL_HIGH] = HIGH;
}

void Set_Frequency(float frequency)
{
	/* Frequency_PWM = Frequency_CLK/((APRx+1)*(PSCx+1))
	 * Frequency_PWM = It is the input of function that user want to set
	 * Frequency_CLK = 72 MHz
	 * APRx = Auto-reload register
	 * PSCx = The value of prescaler */
	volatile static uint32_t PWM_APR;  //start PWM-Frequency with 100Hz
	PWM_APR = (PWM_CLK/frequency) - 1 ;
	TIM3->ARR = PWM_APR;
}

void Set_DutyCycle(float Ton)
{
	/* DutyCycle = CCRx/APRx = Ton/(Ton + Toff)
	 * CCRx = The value for compare */
	float dutycycle = (Ton/1000.000) * (Frequency/1.000);

	//assert(((dutycycle*100) >= 0.0F) && ((dutycycle*100) <= 100.0F));

	uint32_t newRegVal = (uint32_t)roundf((float)(TIM3->ARR + 1) * dutycycle);

    //In case of the dutycycle being calculated as higher than the reload register, cap it to the reload register
	if(newRegVal > TIM3->ARR)
		newRegVal = TIM3->ARR;

	TIM3->CCR1 = (uint32_t)(roundf(newRegVal));
	//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, TIM3->CCR1);
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

  /*first uncomment ValuInit_Write_EE() to write initial values,
  then comment it and upload the program again on the micro*/
  //ValuInit_Write_EE();
  ValuInit_Read_EE();


  // Initial values without eeprom
  /*values_Alfa[1] = EEValues_Alfa[1];
  values_Alfa[2] = EEValues_Alfa[2];
  values_Alfa[3] = EEValues_Alfa[3];
  values_Alfa[4] = EEValues_Alfa[4];
  values_Alfa[5] = EEValues_Alfa[5];
  values_Alfa[6] = EEValues_Alfa[6];

  values_Beta[1] = EEValues_Beta[1];
  values_Beta[2] = EEValues_Beta[2];
  values_Beta[3] = EEValues_Beta[3];
  values_Beta[4] = EEValues_Beta[4];
  values_Beta[5] = EEValues_Beta[5];
  values_Beta[6] = EEValues_Beta[6];

  values_Teta[1] = EEValues_Teta[1];
  values_Teta[2] = EEValues_Teta[2];
  values_Teta[3] = EEValues_Teta[3];
  values_Teta[4] = EEValues_Teta[4];
  values_Teta[5] = EEValues_Teta[5];
  values_Teta[6] = EEValues_Teta[6];

  values_Delta[1] = EEValues_Delta[1];
  values_Delta[2] = EEValues_Delta[2];
  values_Delta[3] = EEValues_Delta[3];
  values_Delta[4] = EEValues_Delta[4];
  values_Delta[5] = EEValues_Delta[5];
  values_Delta[6] = EEValues_Delta[6];

  values_Gama[1] = EEValues_Gama[1];
  values_Gama[2] = EEValues_Gama[2];
  values_Gama[3] = EEValues_Gama[3];
  values_Gama[4] = EEValues_Gama[4];
  values_Gama[5] = EEValues_Gama[5];
  values_Gama[6] = EEValues_Gama[6];

  values_Custom[1] = EEValues_Custom[1];
  values_Custom[2] = EEValues_Custom[2];
  values_Custom[3] = EEValues_Custom[3];
  values_Custom[4] = EEValues_Custom[4];
  values_Custom[5] = EEValues_Custom[5];
  values_Custom[6] = EEValues_Custom[6];*/

  //start encoder timer 1 for reading rotary encoder
  HAL_TIM_Encoder_Start(&htim1, TIM_CHANNEL_ALL);

  //create different flags base on 1ms timer 7
  //__HAL_TIM_ENABLE_IT(&htim7, TIM_IT_UPDATE );
  HAL_TIM_Base_Start_IT(&htim7);

  //create PWM and change Frequency and Duty Cycle
  __HAL_TIM_ENABLE_IT(&htim3, TIM_IT_UPDATE );
  //HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
  //Set_Frequency(Frequency);
  //Set_DutyCycle(DutyCycle);

  volatile char input;// holds the current keys state.
  uint8_t j;

  char (*pStateFunc)(char);
  // Initial state variables
  uint8_t state = ST_STANDBY;
  pStateFunc = StandBy;

  memcpy(&StateNextVar, &main_state_nextVariable, sizeof(main_state_nextVariable));
  memcpy(&StateVar, &main_stateVariable, sizeof(main_stateVariable));
  memcpy(&StateMenu, &main_menuState, sizeof(main_menuState));

  LCD_Init();
  LCD_Clear();
  LCD_DisplayOn();
  LCD_BL_SET(1);
  //uint8_t Blink =1;

  /*char lcdBufferIA[15];
  uint8_t Number=0;
  int16_t UpLimit_DEC = round((EEValues_Alfa[2].downLimit - (int)EEValues_Alfa[2].downLimit)*10);
  sprintf(lcdBufferIA,"%-3d", UpLimit_DEC);
  LCD_Puts(1,0, lcdBufferIA);*/
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*if (fLcdError500ms==true)
	  {

		  if (Blink == 1)
		  {
		     LCD_BL_SET(0);
		     Blink=2;
		  }
		  else if(Blink == 2)
		  {
			  LCD_BL_SET(1);
			  Blink=1;
		  }

		  fLcdError500ms=false;
	  }*/
		encoder_counter = __HAL_TIM_GET_COUNTER(&htim1)/4;
		diff = encoder_counter - last_count;  // Calculate the difference between the current count and the last count

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

		if ( flag10ms == true )
		{
			debounce();  // Update button_state.

			//if (buzzer[BUZZ_NUM])
				//ToggleBuzzer(buzzer);

			flag10ms = false;
		}

		if ( flag1ms == true && flagSetPWM == true )
		{
			CountRamp++;
			if( CountRamp > StepRamp )
			{
				CountRamp = 0;
				flagSetPWM = false;
				//Set_DutyCycle(DutyCycle);
			}
			else
			    Set_DutyCycle( (Time_ON/StepRamp) * CountRamp );

			flag1ms = false;
		}
	  debounce();
	  HAL_Delay(10);
		// Read input and change states
		buttonMask=KEY_MASK;
		buttonMask &= buttons_release;
		buttons_release ^= buttonMask;
		input = buttonMask | ((flagRotaryCCW*ROTARY_CCW_DIR)) | ((flagRotaryCW*ROTARY_CW_DIR)); // Read buttons
		flagRotaryCW = false;
		flagRotaryCCW = false;
	    //Set buzzer to beep if one button is pressed.
		if (input)
		{
			buzzer[BUZZ_NUM] = 1;
			buzzer[BUZZ_DELAY] = 10;
		}

		if (tapChangerState == 0 || errorVar != 0)
		{
			// When in this state, we must call the state function
			nextState = pStateFunc(input);
			if(errorVar != 0)
				nextState = ST_ERRORS;
			if (nextState != state)
			{
				state = nextState;

				for (uint8_t n = 0; (j=StateMenu[n].state); n++)
				{
					if (j == state)
					{
						pStateFunc = StateMenu[n].pFunc;
						break;
					}
				}

			}
		}

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
  hi2c1.Init.Timing = 0x00101827;
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
  htim7.Init.Prescaler = 72-1;
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
  HAL_GPIO_WritePin(BUZZ_GPIO_Port, BUZZ_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LED4_GPIO_Port, LED4_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pins : LCD_BKL_Pin LED1_Pin LED2_Pin LED3_Pin */
  GPIO_InitStruct.Pin = LCD_BKL_Pin|LED1_Pin|LED2_Pin|LED3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : KEY6_Pin */
  GPIO_InitStruct.Pin = KEY6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(KEY6_GPIO_Port, &GPIO_InitStruct);

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

  /*Configure GPIO pin : BUZZ_Pin */
  GPIO_InitStruct.Pin = BUZZ_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(BUZZ_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : rotaryMiddle_Pin Key4_Pin Key5_Pin */
  GPIO_InitStruct.Pin = rotaryMiddle_Pin|Key4_Pin|Key5_Pin;
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

/* USER CODE END 4 */

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
	
  /* USER CODE BEGIN Callback 1 */
  if (htim->Instance == TIM7) {
	volatile static uint32_t count1ms = 0;
	count1ms++;
	flag1ms = true;

	if ( (count1ms % 10) == 0 )
	{
		//if ( (count1ms % 10) == 0 )
		//{
			flag10ms = true;
			flcd10ms = true;
			flagADC10ms= true;
		//}
		if ( (count1ms % 100) == 0 )
		{
			flag100ms = true;
			flcd100ms = true;
			f2lcd100ms = true;
			fSlope100ms = true;
		}
		if ( (count1ms % 300) == 0 )
		{
			flag300ms = true;
			flcd300ms = true;
		}
		if ( (count1ms % 500) == 0 )
		{
			fLcdError500ms = true;
			flagRefreshValue500ms = true;
			flcd500ms = true;

		}
		if ( (count1ms % 1000) == 0 )
		{
			//flagZeroDetectEr1s = true;
			flag1s = true;
			flcd1s = true;
			fupdate1s = true;
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
  if (htim->Instance == TIM3 && flagUpdatePWM == true) {
	  if(StateVar[cursorLine].IndexMenu==Second_Menu_1)
	  {
		  Set_Frequency(Frequency);  //VA_FREQUENCY
		  Set_DutyCycle(Time_ON);  //VA_TIME
	  }
	  flagSetPWM = true;
	  flagUpdatePWM = false;
  }
  /* USER CODE END Callback 1 */
}

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
