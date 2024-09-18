#ifndef MENU_H_
#define MENU_H_

#include "stm32f3xx_hal.h"
//#include "button.h"

#define BUZZ_NUM	0
#define BUZZ_DELAY	1

// Macro definitions
#define NUMBER_OF_VARIABLES			6
#define NUMBER_OF_MAIN_VARIABLES    7

// #define WELD_RELAY_DELAY			200

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


#define ET_FLT				1
#define ET_DBL				2
#define ET_INT				3
#define ET_FSTR				4 /* string saved in flash */
#define ET_ULNG				5
#define ET_NULL				6 // Nothing to display


// type of WAVE
/*#define WAVE_TYPE_NONE	        0
#define WAVE_TYPE_ALFA	    	1
#define WAVE_TYPE_BETA	        2
#define WAVE_TYPE_GAMA	        3*/

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

#define MAIN_Alfa			    VA_FIRST_VARIABLE
#define MAIN_Beta				VA_SECOND_VARIABLE
#define MAIN_Teta				VA_THIRD_VARIABLE
#define MAIN_Delta              VA_FOURTH_VARIABLE
#define MAIN_Gama               VA_FIFTH_VARIABLE
#define MAIN_Custom             VA_SIXTH_VARIABLE
#define MAIN_CONTACT_US			VA_SEVENTH_VARIABLE

#define VA_VOLTAGE			    VA_EIGHTH_VARIABLE
#define VA_FREQUENCY			VA_NINTH_VARIABLE
#define VA_TIME				    VA_TENTH_VARIABLE
#define VA_RAMP			        VA_ELEVENTH_VARIABLE
#define VA_RYTHM_ON			    VA_TWELFTH_VARIABLE
#define VA_RYTHM_OFF		    VA_THIRTEENTH_VARIABLE

 /////////////////////////////////////////////////////////////////////////////////////////////////////////
unsigned char const updownCharacter[] /*PROGMEM*/ = {0x04,0x0E,0x1F,0x00,0x00,0x1F,0X0E,0x04};// "<>" character
uint8_t const arcSymbol[5][8] /*PROGMEM*/ ={
					{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x18},
					{0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x1C},
					{0x00, 0x00, 0x00, 0x00, 0x12, 0x14, 0x18, 0x1E},
					{0x00, 0x00, 0x08, 0x11, 0x12, 0x14, 0x19, 0x1E},
					{0x00, 0x00, 0x08, 0x11, 0x12, 0x14, 0x19, 0x1E}
					};
uint8_t const line[4][8] /*PROGMEM*/ ={
					{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F},  // line down
					{0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},  // line right
					{0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},  // line up
					{0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01}   // line left
					};
 uint8_t const barGraph[6][8] /*PROGMEM*/ ={
					{0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
					{0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10},
					{0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18},
					{0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C},
					{0x00, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E},
					{0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F}
					};

 /////////////////////////////////////////////////////////////////////////////////////////////////////////

typedef struct {
	union {
		float		fval;
		/*PGM_P*/ const char*		pText[4]; /* 4: maximum number of states; ie "non" ,"Alfa", "Beta" and "Gama". */
		int			ival;
		//uint32_t	ulval;
	};
	int8_t strNum; 	/* variable that determine witch string should be displayed.
					 * using int8_t insteed of uint8_t because if strNum == 0 and strNum--
					 * strNum ==-1 not 255. (to flow control)
					 */
	float downLimit;/* These variables store maximum and minimum of variable or strNum.*/
	float upLimit;
} ValueStruct;

//ValueStruct values[ NUMBER_OF_VARIABLES+1 ];
ValueStruct values_main[ NUMBER_OF_MAIN_VARIABLES+1 ];
ValueStruct values_Alfa[ NUMBER_OF_VARIABLES+1 ];
ValueStruct values_Beta[ NUMBER_OF_VARIABLES+1 ];
ValueStruct values_Teta[ NUMBER_OF_VARIABLES+1 ];
ValueStruct values_Delta[ NUMBER_OF_VARIABLES+1 ];
ValueStruct values_Gama[ NUMBER_OF_VARIABLES+1 ];
ValueStruct values_Custom[ NUMBER_OF_VARIABLES+1 ];

typedef struct /*PROGMEM*/
{
	uint8_t variable;
	uint8_t input;
	uint8_t nextVariable;
	uint8_t cursorLine;  // LCD Line which cursor is pointed to it.
} STATE_NEXTVARIABLE;


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
	char (*pFunc)(char input);
} MENU_STATE;

/////////////////////////////////////////////////////////////////////////////////////////////////////////
/* menu Functions  */
void HLine (char x, char y, uint8_t KindofLine,char mem8);
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
/////////////////////////////////////////////////////////////////////////////////////////////////////////
 /*  First menu definitions  */
const char MAIN_FIRST_VARIABLE[]      = " 1.Alfa";
const char MAIN_SECOND_VARIABLE[]	  = " 2.Beta";
const char MAIN_THIRD_VARIABLE[]	  = " 3.Teta";
const char MAIN_FOURTH_VARIABLE[]	  = " 4.Delta";
const char MAIN_FIFTH_VARIABLE[]	  = " 5.Gama";
const char MAIN_SIXTH_VARIABLE[]	  = " 6.Custom";
const char MAIN_SEVENTH_VARIABLE[]    = " 7.Contact Us";

const STATE_NEXTVARIABLE main_state_nextVariable[] /*PROGMEM*/ = {
//	VARIABLE				    INPUT				NEXT VARIABLE			CURSOR LINE
	{VA_FIRST_VARIABLE,			ROTARY_CW_DIR,		VA_SECOND_VARIABLE,		LCD_LINE2},

	{VA_SECOND_VARIABLE,		ROTARY_CW_DIR,		VA_THIRD_VARIABLE,		LCD_LINE3},
	{VA_SECOND_VARIABLE,		ROTARY_CCW_DIR,		VA_FIRST_VARIABLE,		LCD_LINE1},

	{VA_THIRD_VARIABLE,			ROTARY_CW_DIR,		VA_FOURTH_VARIABLE,		LCD_LINE3},
	{VA_THIRD_VARIABLE,			ROTARY_CCW_DIR,		VA_SECOND_VARIABLE,		LCD_LINE2},

	{VA_FOURTH_VARIABLE,		ROTARY_CW_DIR,		VA_FIFTH_VARIABLE,		LCD_LINE3},
	{VA_FOURTH_VARIABLE,		ROTARY_CCW_DIR,		VA_THIRD_VARIABLE,		LCD_LINE3},

	{VA_FIFTH_VARIABLE,		    ROTARY_CW_DIR,		VA_SIXTH_VARIABLE,		LCD_LINE3},
	{VA_FIFTH_VARIABLE,		    ROTARY_CCW_DIR,		VA_FOURTH_VARIABLE,		LCD_LINE3},

	{VA_SIXTH_VARIABLE,		    ROTARY_CW_DIR,		VA_SEVENTH_VARIABLE,	LCD_LINE4},
	{VA_SIXTH_VARIABLE,		    ROTARY_CCW_DIR,		VA_FIFTH_VARIABLE,		LCD_LINE3},

	{VA_SEVENTH_VARIABLE,	    KEY_BACK,		    VA_FIRST_VARIABLE,		LCD_LINE4},
	{VA_SEVENTH_VARIABLE,       ROTARY_CCW_DIR,		VA_SIXTH_VARIABLE,		LCD_LINE3},

	{0,                         0,					0,					    0}
};

const STATE_VARIABLE main_stateVariable[] /*PROGMEM*/ = {
//	VARIABLE				VARIABLE TEXT			    ADJUST_STATE		P_VALUE_STR		        Element TYPE    IndexMenu
	{MAIN_Alfa,				MAIN_FIRST_VARIABLE,		ST_MAIN_STATE,		&values_main[1],	    ET_NULL,        First_Menu},
	{MAIN_Beta,				MAIN_SECOND_VARIABLE,		ST_MAIN_STATE,		&values_main[2],	    ET_NULL,        First_Menu},
	{MAIN_Teta,				MAIN_THIRD_VARIABLE,		ST_MAIN_STATE,		&values_main[3],	    ET_NULL,        First_Menu},
	{MAIN_Delta,			MAIN_FOURTH_VARIABLE,		ST_MAIN_STATE,		&values_main[4],	    ET_NULL,        First_Menu},
    {MAIN_Gama,				MAIN_FIFTH_VARIABLE,		ST_MAIN_STATE,		&values_main[5],		ET_NULL,        First_Menu},
	{MAIN_Custom,			MAIN_SIXTH_VARIABLE,		ST_MAIN_STATE,		&values_main[6],		ET_NULL,        First_Menu},
	{MAIN_CONTACT_US,		MAIN_SEVENTH_VARIABLE,		ST_CONTACT_US,		&values_main[7],		ET_NULL,        First_Menu},

	{0,							NULL,					0,					NULL,				    0,              0}
};

/******************/
/*! \ FUNCTIONS
******************/
const MENU_STATE main_menuState[] /*PROGMEM*/ = {
//  STATE						STATE TEXT		STATE_FUNC
	{ST_STANDBY,                NULL,			StandBy},
	{ST_MAIN_STATE,             NULL,			MainMenu},
    {ST_CONTACT_US,			    NULL,		    ContactUs},

	{0,							NULL,			NULL}

};

/////////////////////////////////////////////////////////////////////////////////////////////////////////
 /*  Second menu definitions  */

/*! \USER MENU */
const char MT_FIRST_VARIABLE[]		/*PROGMEM*/           = " 1.Voltage";
const char MT_SECOND_VARIABLE[]		/*PROGMEM*/           = " 2.Freq(HZ)";
const char MT_THIRD_VARIABLE[]		/*PROGMEM*/			  = " 3.Time(mS)";
const char MT_FORTH_VARIABLE[]		/*PROGMEM*/			  = " 4.Ramp(S)";
const char MT_FIFTH_VARIABLE[]		/*PROGMEM*/			  = " 5.Rythm on";
const char MT_SIXTH_VARIABLE[]		/*PROGMEM*/			  = " 6.Rythm off";
//const char MT_SEVENTH_VARIABLE[]	/*PROGMEM*/			  = " 7.Contact Us";

const STATE_NEXTVARIABLE state_nextVariable[] /*PROGMEM*/ = {
//	VARIABLE						 INPUT				NEXT VARIABLE				CURSOR LINE
	{VA_EIGHTH_VARIABLE,			 ROTARY_CW_DIR,		VA_NINTH_VARIABLE,		    LCD_LINE2},

	{VA_NINTH_VARIABLE,			     ROTARY_CW_DIR,		VA_TENTH_VARIABLE,			LCD_LINE3},
	{VA_NINTH_VARIABLE,			     ROTARY_CCW_DIR,	VA_EIGHTH_VARIABLE,			LCD_LINE1},

	{VA_TENTH_VARIABLE,			     ROTARY_CW_DIR,		VA_ELEVENTH_VARIABLE,		LCD_LINE3},
	{VA_TENTH_VARIABLE,			     ROTARY_CCW_DIR,	VA_NINTH_VARIABLE,		    LCD_LINE2},

	{VA_ELEVENTH_VARIABLE,		     ROTARY_CW_DIR,		VA_TWELFTH_VARIABLE,		LCD_LINE3},
	{VA_ELEVENTH_VARIABLE,		     ROTARY_CCW_DIR,	VA_TENTH_VARIABLE,			LCD_LINE3},

	{VA_TWELFTH_VARIABLE,		     ROTARY_CW_DIR,		VA_THIRTEENTH_VARIABLE,		LCD_LINE4},
	{VA_TWELFTH_VARIABLE,			 ROTARY_CCW_DIR,	VA_ELEVENTH_VARIABLE,		LCD_LINE3},

	{VA_THIRTEENTH_VARIABLE,		 KEY_BACK,		    VA_EIGHTH_VARIABLE,		    LCD_LINE4},
	{VA_THIRTEENTH_VARIABLE,		 ROTARY_CCW_DIR,	VA_TWELFTH_VARIABLE,	    LCD_LINE3},

	{0,                         0,					0,							0}
};

/*const STATE_VARIABLE stateVariable = {
//	VARIABLE					VARIABLE TEXT			ADJUST_STATE		P_VALUE_STR		        Element TYPE   IndexMenu
	{VA_VOLTAGE,				MT_FIRST_VARIABLE,		ST_STR_CHANGE,		&values_Alfa[1],		ET_FSTR,       Second_Menu_1},
	{VA_FREQUENCY,				MT_SECOND_VARIABLE,		ST_FLT_ADJUST,		&values_Alfa[2],		ET_FLT,        Second_Menu_1},
	{VA_TIME,				    MT_THIRD_VARIABLE,		ST_FLT_ADJUST,		&values_Alfa[3],		ET_FLT,        Second_Menu_1},
	{VA_RAMP,				    MT_FORTH_VARIABLE,		ST_INT_ADJUST,		&values_Alfa[4],		ET_INT,        Second_Menu_1},
	{VA_RYTHM_ON,			    MT_FIFTH_VARIABLE,		ST_INT_ADJUST,		&values_Alfa[5],		ET_INT,        Second_Menu_1},
	{VA_RYTHM_OFFF,			    MT_SIXTH_VARIABLE,		ST_INT_ADJUST,		&values_Alfa[6],		ET_INT,        Second_Menu_1},

	{0,							NULL,					0,					NULL,			        0,             0}
};*/

const STATE_VARIABLE stateVariable_Alfa[] /*PROGMEM*/ = {
//	VARIABLE					VARIABLE TEXT			ADJUST_STATE		P_VALUE_STR		        Element TYPE   IndexMenu
	{VA_VOLTAGE,				MT_FIRST_VARIABLE,		ST_STR_CHANGE,		&values_Alfa[1],		ET_FSTR,       Second_Menu_1},
	{VA_FREQUENCY,				MT_SECOND_VARIABLE,		ST_FLT_ADJUST,		&values_Alfa[2],		ET_FLT,        Second_Menu_1},
	{VA_TIME,				    MT_THIRD_VARIABLE,		ST_FLT_ADJUST,		&values_Alfa[3],		ET_FLT,        Second_Menu_1},
	{VA_RAMP,				    MT_FORTH_VARIABLE,		ST_FLT_ADJUST,		&values_Alfa[4],		ET_FLT,        Second_Menu_1},
	{VA_RYTHM_ON,			    MT_FIFTH_VARIABLE,		ST_INT_ADJUST,		&values_Alfa[5],		ET_INT,        Second_Menu_1},
	{VA_RYTHM_OFF,			    MT_SIXTH_VARIABLE,		ST_INT_ADJUST,		&values_Alfa[6],		ET_INT,        Second_Menu_1},

	{0,							NULL,					0,					NULL,			        0,             0}
};
const STATE_VARIABLE stateVariable_Beta[] /*PROGMEM*/ = {
//	VARIABLE					VARIABLE TEXT			ADJUST_STATE		P_VALUE_STR		        Element TYPE   IndexMenu
	{VA_VOLTAGE,				MT_FIRST_VARIABLE,		ST_STR_CHANGE,		&values_Beta[1],		ET_FSTR,       Second_Menu_2},
	{VA_FREQUENCY,				MT_SECOND_VARIABLE,		ST_FLT_ADJUST,		&values_Beta[2],		ET_FLT,        Second_Menu_2},
	{VA_TIME,				    MT_THIRD_VARIABLE,		ST_FLT_ADJUST,		&values_Beta[3],		ET_FLT,        Second_Menu_2},
	{VA_RAMP,				    MT_FORTH_VARIABLE,		ST_FLT_ADJUST,		&values_Beta[4],		ET_FLT,        Second_Menu_2},
	{VA_RYTHM_ON,			    MT_FIFTH_VARIABLE,		ST_INT_ADJUST,		&values_Beta[5],		ET_INT,        Second_Menu_2},
	{VA_RYTHM_OFF,			    MT_SIXTH_VARIABLE,		ST_INT_ADJUST,		&values_Beta[6],		ET_INT,        Second_Menu_2},

	{0,							NULL,					0,					NULL,			        0,             0}
};
const STATE_VARIABLE stateVariable_Teta[] /*PROGMEM*/ = {
//	VARIABLE					VARIABLE TEXT			ADJUST_STATE		P_VALUE_STR		        Element TYPE   IndexMenu
	{VA_VOLTAGE,				MT_FIRST_VARIABLE,		ST_STR_CHANGE,		&values_Teta[1],		ET_FSTR,       Second_Menu_3},
	{VA_FREQUENCY,				MT_SECOND_VARIABLE,		ST_FLT_ADJUST,		&values_Teta[2],		ET_FLT,        Second_Menu_3},
	{VA_TIME,				    MT_THIRD_VARIABLE,		ST_FLT_ADJUST,		&values_Teta[3],		ET_FLT,        Second_Menu_3},
	{VA_RAMP,				    MT_FORTH_VARIABLE,		ST_FLT_ADJUST,		&values_Teta[4],		ET_FLT,        Second_Menu_3},
	{VA_RYTHM_ON,			    MT_FIFTH_VARIABLE,		ST_INT_ADJUST,		&values_Teta[5],		ET_INT,        Second_Menu_3},
	{VA_RYTHM_OFF,			    MT_SIXTH_VARIABLE,		ST_INT_ADJUST,		&values_Teta[6],		ET_INT,        Second_Menu_3},

	{0,							NULL,					0,					NULL,			        0,             0}
};
const STATE_VARIABLE stateVariable_Delta[] /*PROGMEM*/ = {
//	VARIABLE					VARIABLE TEXT			ADJUST_STATE		P_VALUE_STR		        Element TYPE   IndexMenu
	{VA_VOLTAGE,				MT_FIRST_VARIABLE,		ST_STR_CHANGE,		&values_Delta[1],		ET_FSTR,       Second_Menu_4},
	{VA_FREQUENCY,				MT_SECOND_VARIABLE,		ST_FLT_ADJUST,		&values_Delta[2],		ET_FLT,        Second_Menu_4},
	{VA_TIME,				    MT_THIRD_VARIABLE,		ST_FLT_ADJUST,		&values_Delta[3],		ET_FLT,        Second_Menu_4},
	{VA_RAMP,				    MT_FORTH_VARIABLE,		ST_FLT_ADJUST,		&values_Delta[4],		ET_FLT,        Second_Menu_4},
	{VA_RYTHM_ON,			    MT_FIFTH_VARIABLE,		ST_INT_ADJUST,		&values_Delta[5],		ET_INT,        Second_Menu_4},
	{VA_RYTHM_OFF,			    MT_SIXTH_VARIABLE,		ST_INT_ADJUST,		&values_Delta[6],		ET_INT,        Second_Menu_4},

	{0,							NULL,					0,					NULL,			        0,             0}
};
const STATE_VARIABLE stateVariable_Gama[] /*PROGMEM*/ = {
//	VARIABLE					VARIABLE TEXT			ADJUST_STATE		P_VALUE_STR		        Element TYPE   IndexMenu
	{VA_VOLTAGE,				MT_FIRST_VARIABLE,		ST_STR_CHANGE,		&values_Gama[1],		ET_FSTR,       Second_Menu_5},
	{VA_FREQUENCY,				MT_SECOND_VARIABLE,		ST_FLT_ADJUST,		&values_Gama[2],		ET_FLT,        Second_Menu_5},
	{VA_TIME,				    MT_THIRD_VARIABLE,		ST_FLT_ADJUST,		&values_Gama[3],		ET_FLT,        Second_Menu_5},
	{VA_RAMP,				    MT_FORTH_VARIABLE,		ST_FLT_ADJUST,		&values_Gama[4],		ET_FLT,        Second_Menu_5},
	{VA_RYTHM_ON,			    MT_FIFTH_VARIABLE,		ST_INT_ADJUST,		&values_Gama[5],		ET_INT,        Second_Menu_5},
	{VA_RYTHM_OFF,			    MT_SIXTH_VARIABLE,		ST_INT_ADJUST,		&values_Gama[6],		ET_INT,        Second_Menu_5},

	{0,							NULL,					0,					NULL,			        0,             0}
};
const STATE_VARIABLE stateVariable_Custom[] /*PROGMEM*/ = {
//	VARIABLE					VARIABLE TEXT			ADJUST_STATE		P_VALUE_STR		        Element TYPE   IndexMenu
	{VA_VOLTAGE,				MT_FIRST_VARIABLE,		ST_STR_CHANGE,		&values_Custom[1],		ET_FSTR,       Second_Menu_6},
	{VA_FREQUENCY,				MT_SECOND_VARIABLE,		ST_FLT_ADJUST,		&values_Custom[2],		ET_FLT,        Second_Menu_6},
	{VA_TIME,				    MT_THIRD_VARIABLE,		ST_FLT_ADJUST,		&values_Custom[3],		ET_FLT,        Second_Menu_6},
	{VA_RAMP,				    MT_FORTH_VARIABLE,		ST_FLT_ADJUST,		&values_Custom[4],		ET_FLT,        Second_Menu_6},
	{VA_RYTHM_ON,			    MT_FIFTH_VARIABLE,		ST_INT_ADJUST,		&values_Custom[5],		ET_INT,        Second_Menu_6},
	{VA_RYTHM_OFF,			    MT_SIXTH_VARIABLE,		ST_INT_ADJUST,		&values_Custom[6],		ET_INT,        Second_Menu_6},

	{0,							NULL,					0,					NULL,			        0,             0}
};
/******************/
/*! \ FUNCTIONS
******************/
const MENU_STATE menuState[] /*PROGMEM*/ = {
//  STATE						STATE TEXT		STATE_FUNC
	{ST_STANDBY,                NULL,			StandBy},
	{ST_MAIN_STATE,             NULL,			MainMenu},
	{ST_INT_ADJUST,				NULL,			IntAdjust},
    {ST_FLT_ADJUST,			    NULL,		    FloatAdjust},
	{ST_STR_CHANGE,				NULL,			StringChange},
//	{ST_ERRORS,					NULL,			ErrorHandler},

	{0,							NULL,			NULL}
};

#endif /* MENU_H_ */
