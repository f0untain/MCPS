#ifndef __MENU_H_
#define __MENU_H_

#ifdef __cplusplus
 extern "C" {
#endif
	 
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>
#include "button.h"
#include "lcd.h"
#include "eeprom.h"
#include "pwm.h"
#include "hal.h"
	 
// Macro definitions
#define NUMBER_OF_VARIABLES			6
#define NUMBER_OF_MAIN_VARIABLES    7

#define LCD_LINE_RECENT		4  /*Represent recent cursor line*/
#define LCD_LINE1			0
#define LCD_LINE2			1
#define LCD_LINE3			2
#define LCD_LINE4			3

#define LCD_DownLine		0
#define LCD_RightLine		1
#define LCD_UpLine			2
#define LCD_LeftLine		3

#define LCD_Mem0			0
#define LCD_Mem1			1
#define LCD_Mem2			2
#define LCD_Mem3			3
#define LCD_Mem4			4
#define LCD_Mem5			5
#define LCD_Mem6		    6
#define LCD_Mem7			7

#define LCD_PROGRESS_BAR_LINE  100

//Variable type to show in LCD
#define ET_FLT				1
#define ET_DBL				2
#define ET_INT				3
#define ET_FSTR				4 /* string saved in flash */
#define ET_ULNG				5
#define ET_NULL				6 // Nothing to display

// Voltage Level
#define VOL_LEVEL_NONE	        0
#define VOL_LEVEL_LOW	    	1
#define VOL_LEVEL_MED	        2
#define VOL_LEVEL_HIGH	        3

// Menu state machine states
#define ST_STANDBY                      20

/*----------------------------------------------*/
#define ST_MAIN_STATE					30
/*
\_______________  ________________________/
 				\/								*/
#define VA_FIRST_VARIABLE               1
#define VA_SECOND_VARIABLE		        2
#define VA_THIRD_VARIABLE			    3
#define VA_FOURTH_VARIABLE              4
#define VA_FIFTH_VARIABLE               5
#define VA_SIXTH_VARIABLE               6
#define VA_SEVENTH_VARIABLE             7
#define VA_EIGHTH_VARIABLE              8
#define VA_NINTH_VARIABLE               9
#define VA_TENTH_VARIABLE               10
#define VA_ELEVENTH_VARIABLE            11
#define VA_TWELFTH_VARIABLE             12
#define VA_THIRTEENTH_VARIABLE          13
/*----------------------------------------------*/

#define ST_INT_ADJUST					50

#define ST_FLT_ADJUST                   60

#define ST_STR_CHANGE					70

#define ST_CHANGE_MENU				    80

#define ST_ERRORS						90

#define ST_CONTACT_US				    100

#define First_Menu              200   //Main Menu
#define Second_Menu_1           201   //Sub Menu Alfa
#define Second_Menu_2           202   //Sub Menu Beta
#define Second_Menu_3           203   //Sub Menu Teta
#define Second_Menu_4           204   //Sub Menu Delta
#define Second_Menu_5           205   //Sub Menu Gama
#define Second_Menu_6           206   //Sub Menu Cuestom

//First menu variable name	 
#define MAIN_Alfa			    VA_FIRST_VARIABLE
#define MAIN_Beta				VA_SECOND_VARIABLE
#define MAIN_Teta				VA_THIRD_VARIABLE
#define MAIN_Delta              VA_FOURTH_VARIABLE
#define MAIN_Gama               VA_FIFTH_VARIABLE
#define MAIN_Custom             VA_SIXTH_VARIABLE
#define MAIN_CONTACT_US			VA_SEVENTH_VARIABLE
	 
//Sub menu variable name	 
#define VA_VOLTAGE			    VA_EIGHTH_VARIABLE
#define VA_FREQUENCY			VA_NINTH_VARIABLE
#define VA_TIME				    VA_TENTH_VARIABLE
#define VA_RAMP			        VA_ELEVENTH_VARIABLE
#define VA_RYTHM_ON			    VA_TWELFTH_VARIABLE
#define VA_RYTHM_OFF		    VA_THIRTEENTH_VARIABLE

const char NON[]	= "None";
const char LOW[]	= "LOW ";
const char MED[] 	= "MED ";
const char HIGH[]   = "HIGH";
	 
/*  First menu definitions  */
const char MAIN_FIRST_VARIABLE[]      = " 1.Alfa";
const char MAIN_SECOND_VARIABLE[]	  = " 2.Beta";
const char MAIN_THIRD_VARIABLE[]	  = " 3.Teta";
const char MAIN_FOURTH_VARIABLE[]	  = " 4.Delta";
const char MAIN_FIFTH_VARIABLE[]	  = " 5.Gama";
const char MAIN_SIXTH_VARIABLE[]	  = " 6.Custom";
const char MAIN_SEVENTH_VARIABLE[]    = " 7.Contact Us";
	
/*  Second menu definitions  */
const char MT_FIRST_VARIABLE[]	  = " 1.Voltage";
const char MT_SECOND_VARIABLE[]	  = " 2.Freq(HZ)";
const char MT_THIRD_VARIABLE[]    = " 3.Time(mS)";
const char MT_FORTH_VARIABLE[]    = " 4.Ramp(S)";
const char MT_FIFTH_VARIABLE[]	  = " 5.Rythm on";
const char MT_SIXTH_VARIABLE[]	  = " 6.Rythm off";	 
	 
typedef struct {
	union {
		float		fval;
		/*PGM_P*/ const char*		pText[4]; /* 4: maximum number of states; ie "non" ,"Alfa", "Beta" and "Gama". */
		int			ival;
		//uint32_t	ulval;
	};
	int8_t strNum; /* variable that determine witch string should be displayed.
		 			* using int8_t insteed of uint8_t because if strNum == 0 and strNum--
		 			* strNum ==-1 not 255. (to flow control)
		 			*/
	float downLimit; /* These variables store maximum and minimum of variable or strNum.*/
	float upLimit;
} ValueStruct;

typedef struct /*PROGMEM*/
{
	uint8_t variable;
	uint8_t input;
	uint8_t nextVariable;
	uint8_t cursorLine; // LCD Line which cursor is pointed to it.
} STATE_NEXTVARIABLE;
//const STATE_NEXTVARIABLE main_state_nextVariable[8];

typedef struct /*PROGMEM*/
{
	uint8_t variable;
	/*PGM_P*/ const char* pText;
	uint8_t adjustState;
	ValueStruct *pValueStruct;
	uint8_t elementType;
	uint8_t IndexMenu; //follow the menu and sub menus

} STATE_VARIABLE;

typedef struct /*PROGMEM*/
{
	unsigned char state;
	/*PGM_P*/ const char* pText;
	char(*pFunc)(char input);
} MENU_STATE;
	
extern volatile bool	flagRefreshValue500ms,
	 			        flcd100ms,
	 			        f2lcd100ms,
	 			        flcd500ms,
	 			        flagUpdatePWM;	 
	 
class Menu
{
	public:
	Menu();
	~Menu();
	static char enterFunction;
	uint8_t variable;
	// uint8_t currentVariable;// holds the current variable number in order to table, according to "menu.h"
	uint8_t cursorLine; // holds the cursor line, according to "menu.h"
	const char* statetext[4]; // Holds 4 const char pointer to display in lines of LCD.
	uint8_t elementType[4]; // Holds 4 types for values of each variable that display in lines of LCD.
	uint8_t reWarning = 1; //for exhibition of warnings and DisplayRunMenu.
	uint8_t refreshLcd; //for refreshing LCD.
	uint16_t errorVar = 0x00, warningVar = 0x00; // Each bit of this variable corresponds to an error state.
	static char count1;

	ValueStruct *pValues[4]; // Holds 4 ValueStruct pointer that point to a value for each variable.

	ValueStruct values_main[NUMBER_OF_MAIN_VARIABLES + 1];
	ValueStruct values_Alfa[NUMBER_OF_VARIABLES + 1];
	ValueStruct values_Beta[NUMBER_OF_VARIABLES + 1];
	ValueStruct values_Teta[NUMBER_OF_VARIABLES + 1];
	ValueStruct values_Delta[NUMBER_OF_VARIABLES + 1];
	ValueStruct values_Gama[NUMBER_OF_VARIABLES + 1];
	ValueStruct values_Custom[NUMBER_OF_VARIABLES + 1];
	
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
	
	STATE_NEXTVARIABLE main_state_nextVariable[15] = {
		//	VARIABLE		   INPUT		    NEXT VARIABLE		CURSOR LINE
		{ VA_FIRST_VARIABLE,   ROTARY_CW_DIR,   VA_SECOND_VARIABLE,  LCD_LINE2 },

		{ VA_SECOND_VARIABLE,  ROTARY_CW_DIR,   VA_THIRD_VARIABLE,   LCD_LINE3 },
		{ VA_SECOND_VARIABLE,  ROTARY_CCW_DIR,  VA_FIRST_VARIABLE,   LCD_LINE1 },

		{ VA_THIRD_VARIABLE,   ROTARY_CW_DIR,   VA_FOURTH_VARIABLE,  LCD_LINE3 },
		{ VA_THIRD_VARIABLE,   ROTARY_CCW_DIR,  VA_SECOND_VARIABLE,  LCD_LINE2 },
						   
		{ VA_FOURTH_VARIABLE,  ROTARY_CW_DIR,   VA_FIFTH_VARIABLE,   LCD_LINE3 },
		{ VA_FOURTH_VARIABLE,  ROTARY_CCW_DIR,  VA_THIRD_VARIABLE,   LCD_LINE3 },

		{ VA_FIFTH_VARIABLE,   ROTARY_CW_DIR,   VA_SIXTH_VARIABLE,   LCD_LINE3 },
		{ VA_FIFTH_VARIABLE,   ROTARY_CCW_DIR,  VA_FOURTH_VARIABLE,  LCD_LINE3 },
						   
		{ VA_SIXTH_VARIABLE,   ROTARY_CW_DIR,   VA_SEVENTH_VARIABLE, LCD_LINE4 },
		{ VA_SIXTH_VARIABLE,   ROTARY_CCW_DIR,  VA_FIFTH_VARIABLE,   LCD_LINE3 },

		{ VA_SEVENTH_VARIABLE, KEY_BACK,        VA_FIRST_VARIABLE,   LCD_LINE4 },
		{ VA_SEVENTH_VARIABLE, ROTARY_CCW_DIR,  VA_SIXTH_VARIABLE,   LCD_LINE3 },

		{ 0,                   0,               0,                   0 }
    };
	STATE_VARIABLE main_stateVariable[9] /*PROGMEM*/ = {
		//	VARIABLE	    VARIABLE TEXT		    ADJUST_STATE	P_VALUE_STR       Element TYPE   IndexMenu
		{ MAIN_Alfa,        MAIN_FIRST_VARIABLE,    ST_MAIN_STATE,  &values_main[1],  ET_NULL,       First_Menu },
		{ MAIN_Beta,        MAIN_SECOND_VARIABLE,   ST_MAIN_STATE,  &values_main[2],  ET_NULL,       First_Menu },
		{ MAIN_Teta,        MAIN_THIRD_VARIABLE,    ST_MAIN_STATE,  &values_main[3],  ET_NULL,       First_Menu },
		{ MAIN_Delta,       MAIN_FOURTH_VARIABLE,   ST_MAIN_STATE,  &values_main[4],  ET_NULL,       First_Menu },
		{ MAIN_Gama,        MAIN_FIFTH_VARIABLE,    ST_MAIN_STATE,  &values_main[5],  ET_NULL,       First_Menu },
		{ MAIN_Custom,      MAIN_SIXTH_VARIABLE,    ST_MAIN_STATE,  &values_main[6],  ET_NULL,       First_Menu },
		{ MAIN_CONTACT_US,  MAIN_SEVENTH_VARIABLE,  ST_CONTACT_US,  &values_main[7],  ET_NULL,       First_Menu },

		{ 0,                NULL,                   0,              NULL,             0,             0 }
	};
    MENU_STATE main_menuState[5] /*PROGMEM*/ = {
		//  STATE		  STATE TEXT   STATE_FUNC
		{ ST_STANDBY,     NULL,        StandBy },
		{ ST_MAIN_STATE,  NULL,        MainMenu },
		{ ST_CONTACT_US,  NULL,        ContactUs },

		{ 0,              NULL,        NULL }

	};
	STATE_NEXTVARIABLE state_nextVariable[13] /*PROGMEM*/ = {
		//	VARIABLE			  INPUT			  NEXT VARIABLE		      CURSOR LINE
		{ VA_EIGHTH_VARIABLE,     ROTARY_CW_DIR,  VA_NINTH_VARIABLE,      LCD_LINE2 },

		{ VA_NINTH_VARIABLE,      ROTARY_CW_DIR,  VA_TENTH_VARIABLE,      LCD_LINE3 },
		{ VA_NINTH_VARIABLE,      ROTARY_CCW_DIR, VA_EIGHTH_VARIABLE,     LCD_LINE1 },

		{ VA_TENTH_VARIABLE,      ROTARY_CW_DIR,  VA_ELEVENTH_VARIABLE,   LCD_LINE3 },
		{ VA_TENTH_VARIABLE,      ROTARY_CCW_DIR, VA_NINTH_VARIABLE,      LCD_LINE2 },

		{ VA_ELEVENTH_VARIABLE,   ROTARY_CW_DIR,  VA_TWELFTH_VARIABLE,    LCD_LINE3 },
		{ VA_ELEVENTH_VARIABLE,   ROTARY_CCW_DIR, VA_TENTH_VARIABLE,      LCD_LINE3 },

		{ VA_TWELFTH_VARIABLE,    ROTARY_CW_DIR,  VA_THIRTEENTH_VARIABLE, LCD_LINE4 },
		{ VA_TWELFTH_VARIABLE,    ROTARY_CCW_DIR, VA_ELEVENTH_VARIABLE,   LCD_LINE3 },

		{ VA_THIRTEENTH_VARIABLE, KEY_BACK,       VA_EIGHTH_VARIABLE,     LCD_LINE4 },
		{ VA_THIRTEENTH_VARIABLE, ROTARY_CCW_DIR, VA_TWELFTH_VARIABLE,    LCD_LINE3 },

		{ 0,                      0,              0,                      0 }
	};
	STATE_VARIABLE stateVariable_Alfa[8] /*PROGMEM*/ = {
		//	VARIABLE      VARIABLE TEXT		    ADJUST_STATE     P_VALUE_STR		Element TYPE   IndexMenu
		{ VA_VOLTAGE,     MT_FIRST_VARIABLE,    ST_STR_CHANGE,   &values_Alfa[1],   ET_FSTR,       Second_Menu_1 },
		{ VA_FREQUENCY,   MT_SECOND_VARIABLE,   ST_FLT_ADJUST,   &values_Alfa[2],   ET_FLT,        Second_Menu_1 },
		{ VA_TIME,        MT_THIRD_VARIABLE,    ST_FLT_ADJUST,   &values_Alfa[3],   ET_FLT,        Second_Menu_1 },
		{ VA_RAMP,        MT_FORTH_VARIABLE,    ST_FLT_ADJUST,   &values_Alfa[4],   ET_FLT,        Second_Menu_1 },
		{ VA_RYTHM_ON,    MT_FIFTH_VARIABLE,    ST_INT_ADJUST,   &values_Alfa[5],   ET_INT,        Second_Menu_1 },
		{ VA_RYTHM_OFF,   MT_SIXTH_VARIABLE,    ST_INT_ADJUST,   &values_Alfa[6],   ET_INT,        Second_Menu_1 },

		{ 0,              NULL,                 0,               NULL,              0,             0 }
	};
	STATE_VARIABLE stateVariable_Beta[8] /*PROGMEM*/ = {
		//	VARIABLE      VARIABLE TEXT		    ADJUST_STATE     P_VALUE_STR		Element TYPE   IndexMenu
		{ VA_VOLTAGE,     MT_FIRST_VARIABLE,    ST_STR_CHANGE,   &values_Beta[1],   ET_FSTR,       Second_Menu_2 },
		{ VA_FREQUENCY,   MT_SECOND_VARIABLE,   ST_FLT_ADJUST,   &values_Beta[2],   ET_FLT,        Second_Menu_2 },
		{ VA_TIME,        MT_THIRD_VARIABLE,    ST_FLT_ADJUST,   &values_Beta[3],   ET_FLT,        Second_Menu_2 },
		{ VA_RAMP,        MT_FORTH_VARIABLE,    ST_FLT_ADJUST,   &values_Beta[4],   ET_FLT,        Second_Menu_2 },
		{ VA_RYTHM_ON,    MT_FIFTH_VARIABLE,    ST_INT_ADJUST,   &values_Beta[5],   ET_INT,        Second_Menu_2 },
		{ VA_RYTHM_OFF,   MT_SIXTH_VARIABLE,    ST_INT_ADJUST,   &values_Beta[6],   ET_INT,        Second_Menu_2 },

		{ 0,              NULL,                 0,               NULL,              0,             0 }
	};
	STATE_VARIABLE stateVariable_Teta[8] /*PROGMEM*/ = {
		//	VARIABLE      VARIABLE TEXT		    ADJUST_STATE     P_VALUE_STR		Element TYPE   IndexMenu
		{ VA_VOLTAGE,     MT_FIRST_VARIABLE,    ST_STR_CHANGE,   &values_Teta[1],   ET_FSTR,       Second_Menu_3 },
		{ VA_FREQUENCY,   MT_SECOND_VARIABLE,   ST_FLT_ADJUST,   &values_Teta[2],   ET_FLT,        Second_Menu_3 },
		{ VA_TIME,        MT_THIRD_VARIABLE,    ST_FLT_ADJUST,   &values_Teta[3],   ET_FLT,        Second_Menu_3 },
		{ VA_RAMP,        MT_FORTH_VARIABLE,    ST_FLT_ADJUST,   &values_Teta[4],   ET_FLT,        Second_Menu_3 },
		{ VA_RYTHM_ON,    MT_FIFTH_VARIABLE,    ST_INT_ADJUST,   &values_Teta[5],   ET_INT,        Second_Menu_3 },
		{ VA_RYTHM_OFF,   MT_SIXTH_VARIABLE,    ST_INT_ADJUST,   &values_Teta[6],   ET_INT,        Second_Menu_3 },

		{ 0,              NULL,                 0,               NULL,              0,             0 }
	};
	STATE_VARIABLE stateVariable_Delta[8] /*PROGMEM*/ = {
		//	VARIABLE      VARIABLE TEXT		    ADJUST_STATE     P_VALUE_STR		Element TYPE   IndexMenu
		{ VA_VOLTAGE,     MT_FIRST_VARIABLE,    ST_STR_CHANGE,   &values_Delta[1],   ET_FSTR,       Second_Menu_4 },
		{ VA_FREQUENCY,   MT_SECOND_VARIABLE,   ST_FLT_ADJUST,   &values_Delta[2],   ET_FLT,        Second_Menu_4 },
		{ VA_TIME,        MT_THIRD_VARIABLE,    ST_FLT_ADJUST,   &values_Delta[3],   ET_FLT,        Second_Menu_4 },
		{ VA_RAMP,        MT_FORTH_VARIABLE,    ST_FLT_ADJUST,   &values_Delta[4],   ET_FLT,        Second_Menu_4 },
		{ VA_RYTHM_ON,    MT_FIFTH_VARIABLE,    ST_INT_ADJUST,   &values_Delta[5],   ET_INT,        Second_Menu_4 },
		{ VA_RYTHM_OFF,   MT_SIXTH_VARIABLE,    ST_INT_ADJUST,   &values_Delta[6],   ET_INT,        Second_Menu_4 },

		{ 0,              NULL,                 0,               NULL,              0,             0 }
	};
	STATE_VARIABLE stateVariable_Gama[8] /*PROGMEM*/ = {
		//	VARIABLE      VARIABLE TEXT		    ADJUST_STATE     P_VALUE_STR		Element TYPE   IndexMenu
		{ VA_VOLTAGE,     MT_FIRST_VARIABLE,    ST_STR_CHANGE,   &values_Gama[1],   ET_FSTR,       Second_Menu_5 },
		{ VA_FREQUENCY,   MT_SECOND_VARIABLE,   ST_FLT_ADJUST,   &values_Gama[2],   ET_FLT,        Second_Menu_5 },
		{ VA_TIME,        MT_THIRD_VARIABLE,    ST_FLT_ADJUST,   &values_Gama[3],   ET_FLT,        Second_Menu_5 },
		{ VA_RAMP,        MT_FORTH_VARIABLE,    ST_FLT_ADJUST,   &values_Gama[4],   ET_FLT,        Second_Menu_5 },
		{ VA_RYTHM_ON,    MT_FIFTH_VARIABLE,    ST_INT_ADJUST,   &values_Gama[5],   ET_INT,        Second_Menu_5 },
		{ VA_RYTHM_OFF,   MT_SIXTH_VARIABLE,    ST_INT_ADJUST,   &values_Gama[6],   ET_INT,        Second_Menu_5 },

		{ 0,              NULL,                 0,               NULL,              0,             0 }
	};
	STATE_VARIABLE stateVariable_Custom[8] /*PROGMEM*/ = {
		//	VARIABLE      VARIABLE TEXT		    ADJUST_STATE     P_VALUE_STR		Element TYPE   IndexMenu
		{ VA_VOLTAGE,     MT_FIRST_VARIABLE,    ST_STR_CHANGE,   &values_Custom[1],   ET_FSTR,       Second_Menu_6 },
		{ VA_FREQUENCY,   MT_SECOND_VARIABLE,   ST_FLT_ADJUST,   &values_Custom[2],   ET_FLT,        Second_Menu_6 },
		{ VA_TIME,        MT_THIRD_VARIABLE,    ST_FLT_ADJUST,   &values_Custom[3],   ET_FLT,        Second_Menu_6 },
		{ VA_RAMP,        MT_FORTH_VARIABLE,    ST_FLT_ADJUST,   &values_Custom[4],   ET_FLT,        Second_Menu_6 },
		{ VA_RYTHM_ON,    MT_FIFTH_VARIABLE,    ST_INT_ADJUST,   &values_Custom[5],   ET_INT,        Second_Menu_6 },
		{ VA_RYTHM_OFF,   MT_SIXTH_VARIABLE,    ST_INT_ADJUST,   &values_Custom[6],   ET_INT,        Second_Menu_6 },

		{ 0,              NULL,                 0,               NULL,              0,             0 }
	};
	MENU_STATE menuState[7] /*PROGMEM*/ = {
		//  STATE		  STATE TEXT    STATE_FUNC
		{ ST_STANDBY,     NULL,         StandBy },
		{ ST_MAIN_STATE,  NULL,         MainMenu },
		{ ST_INT_ADJUST,  NULL,         IntAdjust },
		{ ST_FLT_ADJUST,  NULL,         FloatAdjust },
		{ ST_STR_CHANGE,  NULL,         StringChange },

		{ 0,              NULL,         NULL }
	};

	
	STATE_NEXTVARIABLE StateNextVar[15];
	STATE_VARIABLE StateVar[8];
	MENU_STATE StateMenu[5];
	
	//Lcd lcd;
	Lcd* LcdInstance = Lcd::GetInstance();
	Pwm pwm;
	Eeprom eeprom;
	
	/* menu Functions  */
	void HLine(char x, char y, uint8_t KindofLine, char mem8);
	char StandBy(char input);
	char ContactUs(char input);
	unsigned char VariableMachine(unsigned char stimuli);
	char MainMenu(char input);
	char IntAdjust(char input);
	char FloatAdjust(char input);
	char StringChange(char input);
	char Warnings(void);
	char ErrorHandler(char input);
	void WeldDisplay(void);
	void DisplayRunMenu(void);
	void InitValueWriteToEeprom(void);
	void InitValueReadFromEeprom(void);
};	 


#ifdef __cplusplus
}
#endif

#endif   /* __MENU_H */
