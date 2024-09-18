#ifndef __LCD_H_
#define __LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"	
#include <stdint.h>
	
#define _LCD_COLS         20
#define _LCD_ROWS         4
	
/* Commands*/
#define LCD_CLEARDISPLAY        0x01
#define LCD_RETURNHOME          0x02
#define LCD_ENTRYMODESET        0x04
#define LCD_DISPLAYCONTROL      0x08
#define LCD_CURSORSHIFT         0x10
#define LCD_FUNCTIONSET         0x20
#define LCD_SETCGRAMADDR        0x40
#define LCD_SETDDRAMADDR        0x80
/* Flags for display entry mode */
#define LCD_ENTRYRIGHT          0x00
#define LCD_ENTRYLEFT           0x02
#define LCD_ENTRYSHIFTINCREMENT 0x01
#define LCD_ENTRYSHIFTDECREMENT 0x00
/* Flags for display on/off control */
#define LCD_DISPLAYON           0x04
#define LCD_CURSORON            0x02
#define LCD_BLINKON             0x01
/* Flags for display/cursor shift */
#define LCD_DISPLAYMOVE         0x08
#define LCD_CURSORMOVE          0x00
#define LCD_MOVERIGHT           0x04
#define LCD_MOVELEFT            0x00
/* Flags for function set */
#define LCD_8BITMODE            0x10
#define LCD_4BITMODE            0x00
#define LCD_2LINE               0x08
#define LCD_1LINE               0x00
#define LCD_5x10DOTS            0x04
#define LCD_5x8DOTS             0x00

#define  _width                 20
#define  _height                4
typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t currentX;
	uint8_t currentY;
  
} LcdOption;
//LcdOption Options;

void Delay_us(uint16_t us);
void Delay_ms(uint8_t ms);
void RstLow(void);
void RstHigh(void);
void EnLow(void);
void EnHigh(void);
void Init(void);	
int16_t getWidth(void);
int16_t getHeight(void);
void EnBlink(void);
void DisplayOn(void);
void DisplayOff(void);
void Clear(void);
void BackLightSet(void);
void Puts(uint8_t x, uint8_t y, char const *str);
void BlinkOn(void);
void BlinkOff(void);
void CursorOn(void);
void CursorOff(void);
void ScrollLeft(void);
void ScrollRight(void);
void CreateChar(uint8_t location, uint8_t const *data);
void PutCustom(uint8_t x, uint8_t y, uint8_t location);
void Put(uint8_t Data);
void Read_str(char* str, uint8_t x, uint8_t y,uint8_t length);
void Cmd(uint8_t cmd);
void Cmd4bit(uint8_t cmd);
void Data(uint8_t data);
void CursorSet(uint8_t col, uint8_t row);
void HLine(char x, char y, uint8_t KindofLine, char mem8);
//void drawBitmap(char x, char y, const uint8_t *image, char mem8);	
	

#ifdef __cplusplus
}
#endif

#endif  /* __LCD_H */

