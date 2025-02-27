
#ifndef LCD_H
#define LCD_H 
/*
+++   Nima Askari
+++   www.github.com/NimaLTD
+++   www.instagram.com/github.NimaLTD 
+++   Version: 1.1.0
*/


// #include "gpio.h"
#include <stdint.h>

void LCD_Init(void);
void LCD_DisplayOn(void);
void LCD_DisplayOff(void);
void LCD_Clear(void);
void LCD_Puts(uint8_t x, uint8_t y, char const *str);
void LCD_BlinkOn(void);
void LCD_BlinkOff(void);
void LCD_CursorOn(void);
void LCD_CursorOff(void);
void LCD_ScrollLeft(void);
void LCD_ScrollRight(void);
void LCD_CreateChar(uint8_t location, uint8_t const *data);
void LCD_PutCustom(uint8_t x, uint8_t y, uint8_t location);
void LCD_Put(uint8_t Data);
//void LCD_Read_str(char* str, uint8_t x, uint8_t y,uint8_t length);
/*static*/ void LCD_Cmd(uint8_t cmd);
/*static*/ void LCD_Cmd4bit(uint8_t cmd);
/*static*/ void LCD_Data(uint8_t data);
/*static*/ void LCD_CursorSet(uint8_t col, uint8_t row);

#endif

