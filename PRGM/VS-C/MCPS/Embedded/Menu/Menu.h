#ifndef __MENU_H_
#define __MENU_H_

#ifdef __cplusplus
 extern "C" {
#endif

#include "main.h"
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

	 
extern uint16_t errorVar, warningVar; // Each bit of this variable corresponds to an error state.
extern uint8_t cursorLine; // holds the cursor line, according to "menu.h"	 
extern float VariableRamp;
extern float StepRamp;
extern uint16_t CountRamp;
extern float Time_ON;
extern float Frequency;

	 
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


extern STATE_NEXTVARIABLE StateNextVar[15];
extern STATE_VARIABLE StateVar[8];
extern MENU_STATE StateMenu[5];	 
	 
extern volatile bool	flagRefreshValue500ms,
	 			        flcd100ms,
	 			        f2lcd100ms,
	 			        flcd500ms,
	 			        flagUpdatePWM;	 
	 
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
void FirstMenuState(void);	 

#ifdef __cplusplus
}
#endif

#endif   /* __MENU_H */
