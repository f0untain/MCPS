#include "Menu.h"


//Lcd lcd;
//Menu menu;
char Menu::enterFunction = 1;

Menu::Menu()
{
	memcpy(&StateNextVar, &main_state_nextVariable, sizeof(main_state_nextVariable));
	memcpy(&StateVar, &main_stateVariable, sizeof(main_stateVariable));
	memcpy(&StateMenu, &main_menuState, sizeof(main_menuState));
	
}

Menu::~Menu()
{
	
}

char Menu::ContactUs(char input)
{
	static uint8_t enterFunction = 1;
	if (enterFunction == 1)
	{
		LcdInstance->Clear();
		LcdInstance->Puts(1, LCD_LINE1, "Tel:021-66760674");
		LcdInstance->Puts(1, LCD_LINE2, "Tel:09191143937");
		LcdInstance->Puts(1, LCD_LINE3, "website");
		LcdInstance->Puts(2, LCD_LINE4, "Nekoorad.com");
	}
	enterFunction = 0;
	if (input == KEY_STANDBY)
	{
		enterFunction = 1;
		return ST_STANDBY;
	}
	else if (input == KEY_BACK)
	{
		refreshLcd = true; // atention to DisplyRunMenu	function
		variable = ST_CHANGE_MENU;
		enterFunction = 1;
		return ST_MAIN_STATE;
	}

	return ST_CONTACT_US;
}

// It shows weld animation.
void Menu::WeldDisplay(void)
{
	uint8_t ArcNumber = 0;
	while (ArcNumber < 5)
	{
		if (flcd100ms)
		{
			LcdInstance->CreateChar(LCD_Mem7, arcSymbol[ArcNumber]);
			LcdInstance->CursorSet(10, LCD_LINE1);
			LcdInstance->Data(LCD_Mem7);
			ArcNumber++;
			flcd100ms = false;
		}

	}
}

/*! @brief This function displays LCD texts.
*	@note This function displays LCD texts each 500 mS or if when a change in data occurs.
*/
void Menu::DisplayRunMenu(void)
{
	char lcdBuffer[12];

	if (flagRefreshValue500ms == true)
	{
		for (uint8_t n = 0; n < 4; n++)
		{
			/* Display values */
			switch (elementType[n])
			{
			case ET_FLT:	sprintf(lcdBuffer, "%-4.1f", pValues[n]->fval);
				LcdInstance->Puts(14, n, lcdBuffer);
				break;

			case ET_INT:
				sprintf(lcdBuffer, "%-3d", pValues[n]->ival);
				LcdInstance->Puts(14, n, lcdBuffer);
				break;

			case ET_FSTR:
				LcdInstance->Puts(14, n, pValues[n]->pText[pValues[n]->strNum]);
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
		LcdInstance->Clear();
		LcdInstance->CursorSet(0, cursorLine);
		LcdInstance->Data(0x7E); //  "->" character

		for (uint8_t n = 0; n < 4; n++)
		{
			// refresh LcdInstance->
			LcdInstance->Puts(1, n, statetext[n]);

			/* Display values */
			switch (elementType[n])
			{
			case ET_FLT:	sprintf(lcdBuffer, "%-4.1f", pValues[n]->fval);
				            LcdInstance->Puts(14, n, lcdBuffer);
				            break;

			case ET_INT:	sprintf(lcdBuffer, "%-3d", pValues[n]->ival);
				            LcdInstance->Puts(14, n, lcdBuffer);
				            break;

			case ET_FSTR:	LcdInstance->Puts(14, n, pValues[n]->pText[pValues[n]->strNum]);
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
unsigned char Menu::VariableMachine(unsigned char stimuli)
{
	unsigned char nextVariable = variable; // Default stay in same state
	unsigned char i, j;
	uint8_t cursorBuffer;

	for (i = 0; (j = StateNextVar[i].variable); i++)
	{
		if (j == variable &&
				StateNextVar[i].input == stimuli)

		{
			nextVariable = StateNextVar[i].nextVariable;
			cursorBuffer = StateNextVar[i].cursorLine;

			if (cursorBuffer != LCD_LINE_RECENT)
				cursorLine = cursorBuffer;

			break;
		}
	}
	return nextVariable;
}

char Menu::StandBy(char input)
{
	static char x1_dot = 0/*,count1=0*/;
	static uint8_t showARMWELD = 1; // ARMWELD animation in standBy.
	static uint8_t	enterFunction = 1;

	if (enterFunction == 1)
	{
//		buzzer[BUZZ_NUM] = 1;
//		buzzer[BUZZ_DELAY] = 255;
		enterFunction = 0;
	}


	if (reWarning == 0 || warningVar == 0)	//need for show after some second warning occured or not any warnings.
	{
		if (f2lcd100ms)
		{

			// display animation of medinalTeb first time when turn on after that show "STANDBY"
			if (x1_dot >= 6 && count1 < 19)
			{
				count1++;
				if (count1 <= 5)
				{
					HLine(9 + count1, LCD_LINE3, LCD_UpLine, LCD_Mem0);
					HLine(10 - count1, LCD_LINE3, LCD_UpLine, LCD_Mem0);
				}
				else if (count1 == 6)
				{
					HLine(9 + count1, LCD_LINE2, LCD_RightLine, LCD_Mem1);
					HLine(10 - count1, LCD_LINE2, LCD_LeftLine, LCD_Mem2);
				}
				else if (count1 <= 11)
				{
					HLine(21 - count1, LCD_LINE1, LCD_DownLine, LCD_Mem3);
					HLine(count1 - 2, LCD_LINE1, LCD_DownLine, LCD_Mem3);
				}
				else if (count1 <= 12)
					WeldDisplay();

			}
			else if (count1 == 19)
			{
				LcdInstance->Clear();
				LcdInstance->Puts(7, LCD_LINE2, "STANDBY");
				showARMWELD = false;
				//					WELD_FAN_OFF;   // turn off fan when ARMWELD animation finish.
				count1++; // it runs just first time this (if).
			}
			f2lcd100ms = false;
		}

		if (flcd500ms)
		{
			if (showARMWELD == true)
			{
				//					WELD_FAN_ON;   // turn on fan when the device  is turning on.
				if (x1_dot < 6)
				{
					LcdInstance->Clear();
					x1_dot++;
				}
				LcdInstance->Puts(x1_dot, 1, "NEKOO");
				LcdInstance->Puts((17 - x1_dot), 1, "RAD");
			}
			flcd500ms = false;
		}

	}
	if (input == KEY_STANDBY)
	{
		enterFunction = 1;
		showARMWELD = false; // because of don't show ARMWELD when come from RunMeno.
		return ST_MAIN_STATE;
	}

	return ST_STANDBY;
}

char Menu::MainMenu(char input)
{
	uint8_t j;
	static uint8_t nextVariable = VA_FIRST_VARIABLE;
	static uint8_t cursor_variable;
	static uint8_t adjustState = ST_MAIN_STATE;
	static uint8_t cursor_main = 0;
	//static char enterFunction = 1;

	if (nextVariable != variable)
	{
		variable = nextVariable;
		refreshLcd = true; // atention to DisplyRunMenu	function
		for (uint8_t currentVariable = 0; (j = StateVar[currentVariable].variable); currentVariable++)
		{
			/* macro to determine which variable must be displayed in first line in LcdInstance-> */
#define FIRST_LINE_VAR		currentVariable-cursorLine

			if (j == variable)
			{
				adjustState = StateVar[currentVariable].adjustState;
				for (uint8_t n = 0; n < 4; n++)
				{
					statetext[n]  = StateVar[FIRST_LINE_VAR + n].pText;
					elementType[n] = StateVar[FIRST_LINE_VAR + n].elementType;
					pValues[n] = (ValueStruct*)StateVar[FIRST_LINE_VAR + n].pValueStruct;
				}
				break;
			}
		}
		if (reWarning == 0 || warningVar == 0)	//need for show after some second warning occured or not any warnings.
			DisplayRunMenu();
	}
	if (input == KEY_STANDBY && StateVar[cursorLine].IndexMenu == First_Menu)
	{
		count1 = 19;
		nextVariable = VA_FIRST_VARIABLE;
		cursorLine = LCD_LINE1;
		variable = VA_SECOND_VARIABLE;
		return ST_STANDBY;
	}
	else if (input == KEY_ACCEPT)
	{
		if (StateVar[cursorLine].IndexMenu == First_Menu)
		{
			if (adjustState == ST_CONTACT_US)
				return adjustState;
			else
			{
				if (variable == MAIN_Alfa)
				{
					memcpy(&StateVar, &stateVariable_Alfa, sizeof(stateVariable_Alfa));
					pwm.Frequency = values_Alfa[2].fval;
					pwm.Time_ON = values_Alfa[3].fval;
					pwm.StepRamp = (values_Alfa[4].fval * 1000) / 1.000;
					pwm.RythmOn = values_Alfa[5].ival;
					pwm.RythmOff = values_Alfa[6].ival;
				}
				else if (variable == MAIN_Beta)
					memcpy(&StateVar, &stateVariable_Beta, sizeof(stateVariable_Beta));
				else if (variable == MAIN_Teta)
					memcpy(&StateVar, &stateVariable_Teta, sizeof(stateVariable_Teta));
				else if (variable == MAIN_Delta)
					memcpy(&StateVar, &stateVariable_Delta, sizeof(stateVariable_Delta));
				else if (variable == MAIN_Gama)
					memcpy(&StateVar, &stateVariable_Gama, sizeof(stateVariable_Gama));
				else if (variable == MAIN_Custom)
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
	else if (input == KEY_BACK)
	{
		memcpy(&StateNextVar, &main_state_nextVariable, sizeof(main_state_nextVariable));
		memcpy(&StateVar, &main_stateVariable, sizeof(main_stateVariable));
		memcpy(&StateMenu, &main_menuState, sizeof(main_menuState));
		cursorLine = cursor_main;
		//nextVariable = cursorLine+1;
		nextVariable = cursor_variable;
		return ST_MAIN_STATE;
	}
	else if (input == KEY_START)
	{
		if (StateVar[cursorLine].IndexMenu != First_Menu)
		{
			// Act like Start Buttom
			if (enterFunction == 1)  // If it is the first time the button is pushed and the variables of frequency and time-on(duty cycle) are set
			{
				//HAL_TIM_PWM_Start_IT(&htim3, TIM_CHANNEL_1);
				//LED1_SET(1);
				flagUpdatePWM = true; // set variable of frequency and duty cycle of PWM on TIM3 Intrupt
				enterFunction = 0;
			}
			/* Act like Stop Buttom */
			else
			{
				//HAL_TIM_PWM_Stop_IT(&htim3, TIM_CHANNEL_1);
				//LED1_SET(0);
				enterFunction = 1;
			}
		}
		return ST_MAIN_STATE;
	}
	else if (input != KEY_NULL)
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
char Menu::IntAdjust(char input)
{
	int16_t valueIA;
	uint8_t j = 1;
	static char enterFunction = 1;

	char lcdBufferIA[15];

	if (enterFunction == 1)
	{
		//pValues[cursorLine]->ival=values[cursorLine+1].ival;
		TIM1->CNT = (pValues[cursorLine]->ival)*ROTARY_SPC; //The amount of variable(that cursor point it)  return to CNT of timer or QDEC .
		enterFunction = 0;
	}
	LcdInstance->CreateChar(0, UpDownCharacter); // "<>" character
	LcdInstance->CursorSet(13, cursorLine);
	LcdInstance->Data(0); //  "<>" character


	/* Check timer valueIA to be in range. */
	valueIA = (int16_t)TIM1->CNT / ROTARY_SPC; // int16_t to avoid of roll over timer valueIA from bottom to top.

	if (valueIA > (int16_t)pValues[cursorLine]->upLimit)
		TIM1->CNT = (int16_t)pValues[cursorLine]->upLimit*ROTARY_SPC;

	else if (valueIA < (int16_t)pValues[cursorLine]->downLimit)
		TIM1->CNT = (int16_t)pValues[cursorLine]->downLimit*ROTARY_SPC;

	sprintf(lcdBufferIA, "%-3d", TIM1->CNT / ROTARY_SPC);
	LcdInstance->Puts(14, cursorLine, lcdBufferIA);

	switch (input)
	{
	case KEY_ACCEPT:
		pValues[cursorLine]->ival = TIM1->CNT / ROTARY_SPC;

		if (variable == VA_RYTHM_ON)
		{
			pwm.RythmOn = pValues[cursorLine]->ival;
		}
		else if (variable == VA_RYTHM_OFF)
		{
			pwm.RythmOff = pValues[cursorLine]->ival;
		}

		if (StateVar[cursorLine].IndexMenu == Second_Menu_1) //if we are in sub menu Alfa:
		{
			for (uint8_t i = 1; i <= NUMBER_OF_VARIABLES; i++)
			{
				eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_2)
		{
			for (uint8_t i = (1 + (1*NUMBER_OF_VARIABLES)); i <= (2*NUMBER_OF_VARIABLES); i++)
			{
				eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_3)
		{
			for (uint8_t i = (1 + (2*NUMBER_OF_VARIABLES)); i <= (3*NUMBER_OF_VARIABLES); i++)
			{
				eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_4)
		{
			for (uint8_t i = (1 + (3*NUMBER_OF_VARIABLES)); i <= (4*NUMBER_OF_VARIABLES); i++)
			{
				eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_5)
		{
			for (uint8_t i = (1 + (4*NUMBER_OF_VARIABLES)); i <= (5*NUMBER_OF_VARIABLES); i++)
			{
				eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_6)
		{
			for (uint8_t i = (1 + (5*NUMBER_OF_VARIABLES)); i <= (6*NUMBER_OF_VARIABLES); i++)
			{
				eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		enterFunction = 1;
		LcdInstance->CursorSet(13, cursorLine);
		LcdInstance->Data(0x20); //  "blank" character
		return ST_MAIN_STATE;

		/*case KEY_BACK:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_MAIN_STATE;*/

	case KEY_STANDBY:
		enterFunction = 1;
		LcdInstance->CursorSet(13, cursorLine);
		LcdInstance->Data(0x20); //  "blank" character
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
char Menu::StringChange(char input)
{
	static char enterFunction = 1;
	static int8_t strNumTemp;
	if (enterFunction == 1)
	{
		strNumTemp = pValues[cursorLine]->strNum;
		enterFunction = 0;
	}

	switch (input)
	{
	case ROTARY_CW_DIR:		strNumTemp++; break;

	case ROTARY_CCW_DIR:	strNumTemp--; break;

	case KEY_ACCEPT:

		// store the temporary adjusted value to the global variable
		pValues[cursorLine]->strNum = strNumTemp;

		if (StateVar[cursorLine].IndexMenu == Second_Menu_1)
		{
			if (variable == VA_VOLTAGE /*&& pValues[cursorLine]->strNum==VOL_LEVEL_LOW*/)
			{
				//LED2_SET(1);
			}
			eeprom.WriteToEeprom(1, 0, (uint8_t*)&values_Alfa[1], sizeof(ValueStruct));
		}

		else if (StateVar[cursorLine].IndexMenu == Second_Menu_2)
		{
			eeprom.WriteToEeprom((1 + (1*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Beta[1], sizeof(ValueStruct));
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_3)
		{
			eeprom.WriteToEeprom((1 + (2*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Teta[1], sizeof(ValueStruct));
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_4)
		{
			eeprom.WriteToEeprom((1 + (3*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Delta[1], sizeof(ValueStruct));
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_5)
		{
			eeprom.WriteToEeprom((1 + (4*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Gama[1], sizeof(ValueStruct));
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_6)
		{
			eeprom.WriteToEeprom((1 + (5*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Custom[1], sizeof(ValueStruct));
		}
		enterFunction = 1;
		LcdInstance->CursorSet(13, cursorLine);
		LcdInstance->Data(0x20); //  "blank" character

		return ST_MAIN_STATE;

		/*case KEY_BACK:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_MAIN_STATE;*/

	case KEY_STANDBY:
		enterFunction = 1;
		LcdInstance->CursorSet(13, cursorLine);
		LcdInstance->Data(0x20); //  "blank" character
		return ST_STANDBY;
	}

	LcdInstance->CreateChar(0, UpDownCharacter); // "<>" character
	LcdInstance->CursorSet(13, cursorLine);
	LcdInstance->Data(0); //  "<>" character


	if (strNumTemp > pValues[cursorLine]->upLimit)
	{
		strNumTemp = pValues[cursorLine]->upLimit;
	}
	else if (strNumTemp < pValues[cursorLine]->downLimit)
	{
		strNumTemp = pValues[cursorLine]->downLimit;
	}
	LcdInstance->Puts(14, cursorLine, pValues[cursorLine]->pText[strNumTemp]);

	return ST_STR_CHANGE;
}

/*! \brief This function adjusts float values.
 *
 * \param input			Buttons state after debouncig.
 *
 * \return nextState    next state according to the current state and button input.
 */
char Menu::FloatAdjust(char input)
{
#define INTEGER_PART	0
#define DECIMAL_PART	1
	static uint8_t mode = INTEGER_PART;
	static int FloatTemp[2];
	static char enterFunction = 1;
	uint8_t j = 1;
	char lcdBufferIA[15];
	//int16_t UpLimit_DEC;
	int16_t DownLimit_DEC;

	if (enterFunction == 1)
	{
		//FloatTemp[DECIMAL_PART] = modf(pValues[cursorLine]->fval, &FloatTemp[INTEGER_PART]);
		FloatTemp[INTEGER_PART] = (int) pValues[cursorLine]->fval;
		FloatTemp[DECIMAL_PART] = round((pValues[cursorLine]->fval - FloatTemp[INTEGER_PART]) * 10);

		//UpLimit_DEC = round((pValues[cursorLine]->upLimit - (int)pValues[cursorLine]->upLimit)*10);
		DownLimit_DEC = round((pValues[cursorLine]->downLimit - (int)pValues[cursorLine]->downLimit) * 10);

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
		FloatTemp[INTEGER_PART] = (int16_t)TIM1->CNT / ROTARY_SPC;
		if (FloatTemp[INTEGER_PART] > (int16_t)pValues[cursorLine]->upLimit)
			TIM1->CNT = (int16_t)pValues[cursorLine]->upLimit*ROTARY_SPC;

		else if (FloatTemp[INTEGER_PART] < (int16_t)pValues[cursorLine]->downLimit)
			TIM1->CNT = (int16_t)pValues[cursorLine]->downLimit*ROTARY_SPC;

		LcdInstance->CreateChar(0, UpDownCharacter); // "<>" character
		   
		LcdInstance->CursorSet(19, cursorLine);
		LcdInstance->Data(0x20); //  "blank" character
		   
		LcdInstance->CursorSet(13, cursorLine);
		LcdInstance->Data(0); //  "<>" character
	}
	else if (mode == DECIMAL_PART)
	{
		FloatTemp[DECIMAL_PART] = (int16_t)TIM1->CNT / ROTARY_SPC;
		if (FloatTemp[DECIMAL_PART] > 9)
			TIM1->CNT = 9*ROTARY_SPC;

		else if (FloatTemp[DECIMAL_PART] < 0)
			TIM1->CNT = 0*ROTARY_SPC;

        //The value of the float number should not be below downlimit of the value
		else if (FloatTemp[DECIMAL_PART] == 0 && FloatTemp[INTEGER_PART] == 0)
			TIM1->CNT = DownLimit_DEC*ROTARY_SPC;

		LcdInstance->CreateChar(0, UpDownCharacter); // "<>" character

		LcdInstance->CursorSet(19, cursorLine);
		LcdInstance->Data(0); //  "<>" character

		LcdInstance->CursorSet(13, cursorLine);
		LcdInstance->Data(0x20); //  "blank" character
	}

	// store the temporary adjusted value to the global variable
	pValues[cursorLine]->fval = (FloatTemp[INTEGER_PART] + (FloatTemp[DECIMAL_PART] / 10.0));
	sprintf(lcdBufferIA, "%-4.1f", pValues[cursorLine]->fval);
	LcdInstance->Puts(14, cursorLine, lcdBufferIA);

	switch (input)
	{
	case ROTARY_CW_DIR:  FloatTemp[mode]++; break;

	case ROTARY_CCW_DIR:  FloatTemp[mode]--; break;

	case KEY_ACCEPT:

		pValues[cursorLine]->fval = (FloatTemp[INTEGER_PART] + (FloatTemp[DECIMAL_PART] / 10.0));
		if (mode == INTEGER_PART)
		{
			mode++;
			TIM1->CNT = (int16_t)(FloatTemp[DECIMAL_PART])*ROTARY_SPC;
			break;
		}
		else if (mode == DECIMAL_PART)
		{
			if (variable == VA_FREQUENCY)
			{
				pwm.Frequency = (pValues[cursorLine]->fval) / 1.000;
			}
			else if (variable == VA_TIME)
			{
				pwm.Time_ON = (pValues[cursorLine]->fval / 1.000);
			}
			else if (variable == VA_RAMP /*&& pValues[cursorLine]->ival==3*/)
			{
				pwm.StepRamp = (pValues[cursorLine]->fval * 1000) / 1.000;
			}

			if (enterFunction == 0 && variable != VA_RAMP) // change variables in the RUN time
				flagUpdatePWM = true;

			if (StateVar[cursorLine].IndexMenu == Second_Menu_1)
			{
				for (uint8_t i = 1; i <= NUMBER_OF_VARIABLES; i++)
				{
					eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_2)
			{
				for (uint8_t i = (1 + (1*NUMBER_OF_VARIABLES)); i <= (2*NUMBER_OF_VARIABLES); i++)
				{
					eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_3)
			{
				for (uint8_t i = (1 + (2*NUMBER_OF_VARIABLES)); i <= (3*NUMBER_OF_VARIABLES); i++)
				{
					eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_4)
			{
				for (uint8_t i = (1 + (3*NUMBER_OF_VARIABLES)); i <= (4*NUMBER_OF_VARIABLES); i++)
				{
					eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_5)
			{
				for (uint8_t i = (1 + (4*NUMBER_OF_VARIABLES)); i <= (5*NUMBER_OF_VARIABLES); i++)
				{
					eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_6)
			{
				for (uint8_t i = (1 + (5*NUMBER_OF_VARIABLES)); i <= (6*NUMBER_OF_VARIABLES); i++)
				{
					eeprom.WriteToEeprom(i, 0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}

			mode = INTEGER_PART;
			enterFunction = 1;
			LcdInstance->CursorSet(19, cursorLine);
			LcdInstance->Data(0x20); //  "blank" character
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
		LcdInstance->CursorSet(13, cursorLine);
		LcdInstance->Data(0x20); //  "blank" character
		return ST_STANDBY;
	}

	return ST_FLT_ADJUST;
}
void  Menu:: InitValueWriteToEeprom(void)
{
	uint8_t j = 1;
	for (uint8_t i = 1; i <= NUMBER_OF_VARIABLES; i++)
	{
		eeprom.WriteToEeprom(i, 0, (uint8_t*)&EEValues_Alfa[i], sizeof(ValueStruct));
	}
	for (uint8_t i = (1 + (1*NUMBER_OF_VARIABLES)); i <= (2*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.WriteToEeprom(i, 0, (uint8_t*)&EEValues_Beta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (2*NUMBER_OF_VARIABLES)); i <= (3*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.WriteToEeprom(i, 0, (uint8_t*)&EEValues_Teta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (3*NUMBER_OF_VARIABLES)); i <= (4*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.WriteToEeprom(i, 0, (uint8_t*)&EEValues_Delta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (4*NUMBER_OF_VARIABLES)); i <= (5*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.WriteToEeprom(i, 0, (uint8_t*)&EEValues_Gama[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (5*NUMBER_OF_VARIABLES)); i <= (6*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.WriteToEeprom(i, 0, (uint8_t*)&EEValues_Custom[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
}

//Read initial values from external eeprom.
void Menu::InitValueReadFromEeprom(void)
{
	uint8_t j = 1;
	for (uint8_t i = 1; i <= NUMBER_OF_VARIABLES; i++)
	{
		eeprom.ReadFromEeprom(i, 0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
	}
	for (uint8_t i = (1 + (1*NUMBER_OF_VARIABLES)); i <= (2*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.ReadFromEeprom(i, 0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (2*NUMBER_OF_VARIABLES)); i <= (3*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.ReadFromEeprom(i, 0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (3*NUMBER_OF_VARIABLES)); i <= (4*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.ReadFromEeprom(i, 0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (4*NUMBER_OF_VARIABLES)); i <= (5*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.ReadFromEeprom(i, 0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (5*NUMBER_OF_VARIABLES)); i <= (6*NUMBER_OF_VARIABLES); i++)
	{
		eeprom.ReadFromEeprom(i, 0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
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