#ifndef __LCD_H_
#define __LCD_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "hal.h"	
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

typedef struct {
	uint8_t DisplayControl;
	uint8_t DisplayFunction;
	uint8_t DisplayMode;
	uint8_t currentX;
	uint8_t currentY;
  
} LcdOption;

unsigned char const UpDownCharacter[8]  = { 0x04, 0x0E, 0x1F, 0x00, 0x00, 0x1F, 0X0E, 0x04 }; // "<>" character
uint8_t const arcSymbol[5][8]  = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x18 },
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x10, 0x1C },
	{ 0x00, 0x00, 0x00, 0x00, 0x12, 0x14, 0x18, 0x1E },
	{ 0x00, 0x00, 0x08, 0x11, 0x12, 0x14, 0x19, 0x1E },
	{ 0x00, 0x00, 0x08, 0x11, 0x12, 0x14, 0x19, 0x1E }
};
uint8_t const Line[4][8]  = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1F },
	// line down
	{ 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
	// line right
	{ 0x1F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	// line up
	{ 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01 }  
	// line left
};
uint8_t const BarGraph[6][8]  = {
	{ 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },
	{ 0x00, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10, 0x10 },
	{ 0x00, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18, 0x18 },
	{ 0x00, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C, 0x1C },
	{ 0x00, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E, 0x1E },
	{ 0x00, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F, 0x1F }
};

class Lcd
{
	public:
	LcdOption Options;
	
	static Lcd* GetInstance();
private:
	static Lcd* _instance;
	uint16_t _width = 16;
	uint16_t _height = 4;
	Lcd();
	~Lcd();

private: void Delay_us(uint16_t us);
private: void Delay_ms(uint8_t ms);
private: void RstLow(void);
private: void RstHigh(void);
private: void EnLow(void);
private: void EnHigh(void);
private: void Init(void);	
	
public:	int16_t getWidth(void) { return _width; }
public:	int16_t getHeight(void) { return _height; }
public: void EnBlink(void);
public: void DisplayOn(void);
public: void DisplayOff(void);
public: void Clear(void);
public: void BackLightSet(void);
public: void Puts(uint8_t x, uint8_t y, char const *str);
public: void BlinkOn(void);
public: void BlinkOff(void);
public: void CursorOn(void);
public: void CursorOff(void);
public: void ScrollLeft(void);
public: void ScrollRight(void);
public: void CreateChar(uint8_t location, uint8_t const *data);
public: void PutCustom(uint8_t x, uint8_t y, uint8_t location);
public: void Put(uint8_t Data);
//public: void Read_str(char* str, uint8_t x, uint8_t y,uint8_t length);
public: void Cmd(uint8_t cmd);
public: void Cmd4bit(uint8_t cmd);
public: void Data(uint8_t data);
public: void CursorSet(uint8_t col, uint8_t row);
public:	void HLine(char x, char y, uint8_t KindofLine, char mem8);
public:	void drawBitmap(char x, char y, const uint8_t *image, char mem8);	
private: Hal hal;
	
};

#ifdef __cplusplus
}
#endif

#endif  /* __LCD_H */

