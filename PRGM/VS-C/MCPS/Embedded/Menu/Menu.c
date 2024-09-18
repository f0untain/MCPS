#include "Menu.h"

static char enterFunction = 1;
uint8_t variable;
// uint8_t currentVariable;// holds the current variable number in order to table, according to "menu.h"
uint8_t cursorLine; // holds the cursor line, according to "menu.h"
const char* statetext[4]; // Holds 4 const char pointer to display in lines of LCD.
uint8_t elementType[4]; // Holds 4 types for values of each variable that display in lines of LCD.
uint8_t reWarning = 1; //for exhibition of warnings and DisplayRunMenu.
uint8_t refreshLcd; //for refreshing LCD.
uint16_t errorVar = 0x00, warningVar = 0x00; // Each bit of this variable corresponds to an error state.
static char count1;

float VariableRamp = 0.1;
float StepRamp = 0.0;
uint16_t CountRamp = 0;
float Time_ON;
int RythmOn;
int RythmOff;

unsigned char const UpDownCharacter[8] = { 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x1F, 0X0E, 0x04 }; // "<>" character
uint8_t const arcSymbol[5][8] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x18 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x1C },
	{ 0x00, 0x00, 0x00, 0x00, 0x12, 0x14, 0x18, 0x1E },
	{ 0x00, 0x00, 0x08, 0x11, 0x12, 0x14, 0x19, 0x1E },
	{ 0x00, 0x00, 0x08, 0x11, 0x12, 0x14, 0x19, 0x1E }
};
uint8_t const Line[4][8] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F },
	// line down
	{ 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
	// line right
	{ 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	// line up
	{ 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }  
	// line left
};
uint8_t const BarGraph[6][8] = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
	{ 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
	{ 0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C },
	{ 0x00, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E },
	{ 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }
};

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

// It draws lines in lcd.
void HLine(char x, char y, uint8_t KindofLine, char mem8)
{
	CreateChar(mem8, Line[KindofLine]);
	CursorSet(x, y);
	Data(mem8);
}

char ContactUs(char input)
{
	static uint8_t enterFunction = 1;
	if (enterFunction == 1)
	{
		Clear();
		Puts(1, LCD_LINE1, "Tel:021-66760674");
		Puts(1, LCD_LINE2, "Tel:09191143937");
		Puts(1, LCD_LINE3, "website");
		Puts(2, LCD_LINE4, "Nekoorad.com");
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
void WeldDisplay(void)
{
	uint8_t ArcNumber = 0;
	while (ArcNumber < 5)
	{
		if (flcd100ms)
		{
			CreateChar(LCD_Mem7, arcSymbol[ArcNumber]);
			CursorSet(10, LCD_LINE1);
			Data(LCD_Mem7);
			ArcNumber++;
			flcd100ms = false;
		}

	}
}

/*! @brief This function displays LCD texts.
*	@note This function displays LCD texts each 500 mS or if when a change in data occurs.
*/
void DisplayRunMenu(void)
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
				Puts(14, n, lcdBuffer);
				break;

			case ET_INT:
				sprintf(lcdBuffer, "%-3d", pValues[n]->ival);
				Puts(14, n, lcdBuffer);
				break;

			case ET_FSTR:
				Puts(14, n, pValues[n]->pText[pValues[n]->strNum]);
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
		Clear();
		CursorSet(0, cursorLine);
		Data(0x7E); //  "->" character

		for (uint8_t n = 0; n < 4; n++)
		{
			// refresh 
			Puts(1, n, statetext[n]);

			/* Display values */
			switch (elementType[n])
			{
			case ET_FLT:	sprintf(lcdBuffer, "%-4.1f", pValues[n]->fval);
				            Puts(14, n, lcdBuffer);
				            break;

			case ET_INT:	sprintf(lcdBuffer, "%-3d", pValues[n]->ival);
				            Puts(14, n, lcdBuffer);
				            break;

			case ET_FSTR:	Puts(14, n, pValues[n]->pText[pValues[n]->strNum]);
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

char StandBy(char input)
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
				Clear();
				Puts(7, LCD_LINE2, "STANDBY");
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
					Clear();
					x1_dot++;
				}
				Puts(x1_dot, 1, "NEKOO");
				Puts((17 - x1_dot), 1, "RAD");
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

char MainMenu(char input)
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
			/* macro to determine which variable must be displayed in first line in  */
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
					Frequency = values_Alfa[2].fval;
					Time_ON = values_Alfa[3].fval;
					StepRamp = (values_Alfa[4].fval * 1000) / 1.000;
					RythmOn = values_Alfa[5].ival;
					RythmOff = values_Alfa[6].ival;
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
char IntAdjust(char input)
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
	CreateChar(0, UpDownCharacter); // "<>" character
	CursorSet(13, cursorLine);
	Data(0); //  "<>" character


	/* Check timer valueIA to be in range. */
	valueIA = (int16_t)TIM1->CNT / ROTARY_SPC; // int16_t to avoid of roll over timer valueIA from bottom to top.

	if (valueIA > (int16_t)pValues[cursorLine]->upLimit)
		TIM1->CNT = (int16_t)pValues[cursorLine]->upLimit*ROTARY_SPC;

	else if (valueIA < (int16_t)pValues[cursorLine]->downLimit)
		TIM1->CNT = (int16_t)pValues[cursorLine]->downLimit*ROTARY_SPC;

	sprintf(lcdBufferIA, "%-3d", TIM1->CNT / ROTARY_SPC);
	Puts(14, cursorLine, lcdBufferIA);

	switch (input)
	{
	case KEY_ACCEPT:
		pValues[cursorLine]->ival = TIM1->CNT / ROTARY_SPC;

		if (variable == VA_RYTHM_ON)
		{
			RythmOn = pValues[cursorLine]->ival;
		}
		else if (variable == VA_RYTHM_OFF)
		{
			RythmOff = pValues[cursorLine]->ival;
		}

		if (StateVar[cursorLine].IndexMenu == Second_Menu_1) //if we are in sub menu Alfa:
		{
			for (uint8_t i = 1; i <= NUMBER_OF_VARIABLES; i++)
			{
				WriteToEeprom(i, 0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_2)
		{
			for (uint8_t i = (1 + (1*NUMBER_OF_VARIABLES)); i <= (2*NUMBER_OF_VARIABLES); i++)
			{
				WriteToEeprom(i, 0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_3)
		{
			for (uint8_t i = (1 + (2*NUMBER_OF_VARIABLES)); i <= (3*NUMBER_OF_VARIABLES); i++)
			{
				WriteToEeprom(i, 0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_4)
		{
			for (uint8_t i = (1 + (3*NUMBER_OF_VARIABLES)); i <= (4*NUMBER_OF_VARIABLES); i++)
			{
				WriteToEeprom(i, 0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_5)
		{
			for (uint8_t i = (1 + (4*NUMBER_OF_VARIABLES)); i <= (5*NUMBER_OF_VARIABLES); i++)
			{
				WriteToEeprom(i, 0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_6)
		{
			for (uint8_t i = (1 + (5*NUMBER_OF_VARIABLES)); i <= (6*NUMBER_OF_VARIABLES); i++)
			{
				WriteToEeprom(i, 0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
				j++;
				if (j == 7)
					j = 1;
			}
		}
		enterFunction = 1;
		CursorSet(13, cursorLine);
		Data(0x20); //  "blank" character
		return ST_MAIN_STATE;

		/*case KEY_BACK:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_MAIN_STATE;*/

	case KEY_STANDBY:
		enterFunction = 1;
		CursorSet(13, cursorLine);
		Data(0x20); //  "blank" character
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
			WriteToEeprom(1, 0, (uint8_t*)&values_Alfa[1], sizeof(ValueStruct));
		}

		else if (StateVar[cursorLine].IndexMenu == Second_Menu_2)
		{
			WriteToEeprom((1 + (1*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Beta[1], sizeof(ValueStruct));
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_3)
		{
			WriteToEeprom((1 + (2*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Teta[1], sizeof(ValueStruct));
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_4)
		{
			WriteToEeprom((1 + (3*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Delta[1], sizeof(ValueStruct));
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_5)
		{
			WriteToEeprom((1 + (4*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Gama[1], sizeof(ValueStruct));
		}
		else if (StateVar[cursorLine].IndexMenu == Second_Menu_6)
		{
			WriteToEeprom((1 + (5*NUMBER_OF_VARIABLES)), 0, (uint8_t*)&values_Custom[1], sizeof(ValueStruct));
		}
		enterFunction = 1;
		CursorSet(13, cursorLine);
		Data(0x20); //  "blank" character

		return ST_MAIN_STATE;

		/*case KEY_BACK:
			enterFunction = 1;
			LCD_CursorSet(13,cursorLine);
			LCD_Data(0x20);  //  "blank" character
			return ST_MAIN_STATE;*/

	case KEY_STANDBY:
		enterFunction = 1;
		CursorSet(13, cursorLine);
		Data(0x20); //  "blank" character
		return ST_STANDBY;
	}

	CreateChar(0, UpDownCharacter); // "<>" character
	CursorSet(13, cursorLine);
	Data(0); //  "<>" character


	if (strNumTemp > pValues[cursorLine]->upLimit)
	{
		strNumTemp = pValues[cursorLine]->upLimit;
	}
	else if (strNumTemp < pValues[cursorLine]->downLimit)
	{
		strNumTemp = pValues[cursorLine]->downLimit;
	}
	Puts(14, cursorLine, pValues[cursorLine]->pText[strNumTemp]);

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

		CreateChar(0, UpDownCharacter); // "<>" character
		   
		CursorSet(19, cursorLine);
		Data(0x20); //  "blank" character
		   
		CursorSet(13, cursorLine);
		Data(0); //  "<>" character
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

		CreateChar(0, UpDownCharacter); // "<>" character

		CursorSet(19, cursorLine);
		Data(0); //  "<>" character

		CursorSet(13, cursorLine);
		Data(0x20); //  "blank" character
	}

	// store the temporary adjusted value to the global variable
	pValues[cursorLine]->fval = (FloatTemp[INTEGER_PART] + (FloatTemp[DECIMAL_PART] / 10.0));
	sprintf(lcdBufferIA, "%-4.1f", pValues[cursorLine]->fval);
	Puts(14, cursorLine, lcdBufferIA);

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
				Frequency = (pValues[cursorLine]->fval) / 1.000;
			}
			else if (variable == VA_TIME)
			{
				Time_ON = (pValues[cursorLine]->fval / 1.000);
			}
			else if (variable == VA_RAMP /*&& pValues[cursorLine]->ival==3*/)
			{
				StepRamp = (pValues[cursorLine]->fval * 1000) / 1.000;
			}

			if (enterFunction == 0 && variable != VA_RAMP) // change variables in the RUN time
				flagUpdatePWM = true;

			if (StateVar[cursorLine].IndexMenu == Second_Menu_1)
			{
				for (uint8_t i = 1; i <= NUMBER_OF_VARIABLES; i++)
				{
					WriteToEeprom(i, 0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_2)
			{
				for (uint8_t i = (1 + (1*NUMBER_OF_VARIABLES)); i <= (2*NUMBER_OF_VARIABLES); i++)
				{
					WriteToEeprom(i, 0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_3)
			{
				for (uint8_t i = (1 + (2*NUMBER_OF_VARIABLES)); i <= (3*NUMBER_OF_VARIABLES); i++)
				{
					WriteToEeprom(i, 0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_4)
			{
				for (uint8_t i = (1 + (3*NUMBER_OF_VARIABLES)); i <= (4*NUMBER_OF_VARIABLES); i++)
				{
					WriteToEeprom(i, 0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_5)
			{
				for (uint8_t i = (1 + (4*NUMBER_OF_VARIABLES)); i <= (5*NUMBER_OF_VARIABLES); i++)
				{
					WriteToEeprom(i, 0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}
			else if (StateVar[cursorLine].IndexMenu == Second_Menu_6)
			{
				for (uint8_t i = (1 + (5*NUMBER_OF_VARIABLES)); i <= (6*NUMBER_OF_VARIABLES); i++)
				{
					WriteToEeprom(i, 0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
					j++;
					if (j == 7)
						j = 1;
				}
			}

			mode = INTEGER_PART;
			enterFunction = 1;
			CursorSet(19, cursorLine);
			Data(0x20); //  "blank" character
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
		CursorSet(13, cursorLine);
		Data(0x20); //  "blank" character
		return ST_STANDBY;
	}

	return ST_FLT_ADJUST;
}
void   InitValueWriteToEeprom(void)
{
	uint8_t j = 1;
	for (uint8_t i = 1; i <= NUMBER_OF_VARIABLES; i++)
	{
		WriteToEeprom(i, 0, (uint8_t*)&EEValues_Alfa[i], sizeof(ValueStruct));
	}
	for (uint8_t i = (1 + (1*NUMBER_OF_VARIABLES)); i <= (2*NUMBER_OF_VARIABLES); i++)
	{
		WriteToEeprom(i, 0, (uint8_t*)&EEValues_Beta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (2*NUMBER_OF_VARIABLES)); i <= (3*NUMBER_OF_VARIABLES); i++)
	{
		WriteToEeprom(i, 0, (uint8_t*)&EEValues_Teta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (3*NUMBER_OF_VARIABLES)); i <= (4*NUMBER_OF_VARIABLES); i++)
	{
		WriteToEeprom(i, 0, (uint8_t*)&EEValues_Delta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (4*NUMBER_OF_VARIABLES)); i <= (5*NUMBER_OF_VARIABLES); i++)
	{
		WriteToEeprom(i, 0, (uint8_t*)&EEValues_Gama[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (5*NUMBER_OF_VARIABLES)); i <= (6*NUMBER_OF_VARIABLES); i++)
	{
		WriteToEeprom(i, 0, (uint8_t*)&EEValues_Custom[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
}

//Read initial values from external 
void InitValueReadFromEeprom(void)
{
	uint8_t j = 1;
	for (uint8_t i = 1; i <= NUMBER_OF_VARIABLES; i++)
	{
		ReadFromEeprom(i, 0, (uint8_t*)&values_Alfa[i], sizeof(ValueStruct));
	}
	for (uint8_t i = (1 + (1*NUMBER_OF_VARIABLES)); i <= (2*NUMBER_OF_VARIABLES); i++)
	{
		ReadFromEeprom(i, 0, (uint8_t*)&values_Beta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (2*NUMBER_OF_VARIABLES)); i <= (3*NUMBER_OF_VARIABLES); i++)
	{
		ReadFromEeprom(i, 0, (uint8_t*)&values_Teta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (3*NUMBER_OF_VARIABLES)); i <= (4*NUMBER_OF_VARIABLES); i++)
	{
		ReadFromEeprom(i, 0, (uint8_t*)&values_Delta[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (4*NUMBER_OF_VARIABLES)); i <= (5*NUMBER_OF_VARIABLES); i++)
	{
		ReadFromEeprom(i, 0, (uint8_t*)&values_Gama[j], sizeof(ValueStruct));
		j++;
		if (j == 7)
			j = 1;
	}
	for (uint8_t i = (1 + (5*NUMBER_OF_VARIABLES)); i <= (6*NUMBER_OF_VARIABLES); i++)
	{
		ReadFromEeprom(i, 0, (uint8_t*)&values_Custom[j], sizeof(ValueStruct));
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

void FirstMenuState(void)
{
	memcpy(&StateNextVar, &main_state_nextVariable, sizeof(main_state_nextVariable));
	memcpy(&StateVar, &main_stateVariable, sizeof(main_stateVariable));
	memcpy(&StateMenu, &main_menuState, sizeof(main_menuState));
}