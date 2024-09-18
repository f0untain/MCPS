
#include "lcd.h"

Lcd* Lcd::_instance = NULL;
Lcd::Lcd()
{
	Init();
	DisplayOn();
	Clear();
	BackLightSet();
}

Lcd::~Lcd()
{
	
}
Lcd* Lcd::GetInstance()
{
	if (_instance == NULL)
	{
		_instance = new Lcd();
	}
	
	return _instance;
}
/**
 * @brief  Reset pin of LCD is reset.
 */
void Lcd::RstLow(void){ hal.WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_RESET); }

/**
 * @brief  Reset pin of LCD is set.
 */
void Lcd::RstHigh(void){ hal.WritePin(LCD_RS_GPIO_Port, LCD_RS_Pin, GPIO_PIN_SET); }

/**
 * @brief  Enable pin of LCD is reset.
 */
void Lcd::EnLow(void){ hal.WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_RESET); }

/**
 * @brief  Enable pin of LCD is set.
 */
void Lcd::EnHigh(void){ hal.WritePin(LCD_EN_GPIO_Port, LCD_EN_Pin, GPIO_PIN_SET); }

/**
 * @brief  Enable pin of LCD is blinking every 50 usecond.
 */
void Lcd::EnBlink(void){ EnHigh(); Delay_us(50); EnLow(); Delay_us(50); }

/**
 * @brief  Back light pin of LCD is set.
 */
void Lcd::BackLightSet(void){ hal.WritePin(LCD_BKL_GPIO_Port, LCD_BKL_Pin, GPIO_PIN_SET); };

/**
 * @brief  Make usecond delay from sysTick of microcontroller.
 * @param  the us time that was needed for delay.
 */
void Lcd::Delay_us(uint16_t us)
{
  uint32_t  Div = (SysTick->LOAD+1)/1000;
  uint32_t  StartMicros = HAL_GetTick()*1000 + (1000- SysTick->VAL/Div);
  while((HAL_GetTick()*1000 + (1000-SysTick->VAL/Div)-StartMicros < us));  
}

/**
 * @brief  Make usecond delay from sysTick of microcontroller.
 * @param  the ms time that was needed for delay.
 */
void Lcd::Delay_ms(uint8_t ms)
{
  //#if _LCD_USE_FREERTOS==1
  //osDelay(ms);
  //#else
  HAL_Delay(ms);
  //#endif
}

/**
 * @brief  First initilize of lcd mode,
 *                               intrface,
 *                               position,
 *                               font.
 *         this fucntion call one time.
 */
void Lcd::Init(void)
{	
	while(HAL_GetTick()<200)
    Delay_ms(1);
	/* Set cursor pointer to beginning for LCD */
	Options.currentX = 0;
	Options.currentY = 0;
	Options.DisplayFunction = LCD_4BITMODE | LCD_5x8DOTS | LCD_1LINE;
	if (_LCD_ROWS > 1)
		Options.DisplayFunction |= LCD_2LINE;
	/* Try to set 4bit mode */
	Cmd4bit(0x03);
	Delay_ms(5);
	/* Second try */
	Cmd4bit(0x03);
	Delay_ms(5);
	/* Third goo! */
	Cmd4bit(0x03);
	Delay_ms(5);
	/* Set 4-bit interface */
	Cmd4bit(0x02);
	Delay_ms(5);
	/* Set # lines, font size, etc. */
	Cmd(LCD_FUNCTIONSET | Options.DisplayFunction);
	/* Turn the display on with no cursor or blinking default */
	Options.DisplayControl = LCD_DISPLAYON;
	DisplayOn();
	Clear();
	/* Default font directions */
	Options.DisplayMode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDECREMENT;
	Cmd(LCD_ENTRYMODESET | Options.DisplayMode);
	Delay_ms(5);
}

/**
 * @brief  Clear display.
 */
void Lcd::Clear(void)
{
	Cmd(LCD_CLEARDISPLAY);
	Delay_ms(5);
}

/**
 * @brief  Write string on the lcd display.
 * @param  Width position on lcd.
 * @param  Hight positon on lcd.
 * @param  Sting value was needed to write..
 */
void Lcd::Puts(uint8_t x, uint8_t y, char const *str)
{
	CursorSet(x, y);
	while (*str)
  {
		if (Options.currentX >= _LCD_COLS)
    {
			Options.currentX = 0;
			Options.currentY++;
			CursorSet(Options.currentX, Options.currentY);
		}
		if (*str == '\n')
    {
			Options.currentY++;
			CursorSet(Options.currentX, Options.currentY);
		}
    else if (*str == '\r')
    {
			CursorSet(0, Options.currentY);
		}
    else
    {
			Data(*str);
			Options.currentX++;
		}
		str++;
	}
}

void Lcd::DisplayOn(void)
{
	Options.DisplayControl |= LCD_DISPLAYON;
	Cmd(LCD_DISPLAYCONTROL | Options.DisplayControl);
}

void Lcd::DisplayOff(void)
{
	Options.DisplayControl &= ~LCD_DISPLAYON;
	Cmd(LCD_DISPLAYCONTROL | Options.DisplayControl);
}

void Lcd::BlinkOn(void)
{
	Options.DisplayControl |= LCD_BLINKON;
	Cmd(LCD_DISPLAYCONTROL | Options.DisplayControl);
}

void Lcd::BlinkOff(void)
{
	Options.DisplayControl &= ~LCD_BLINKON;
	Cmd(LCD_DISPLAYCONTROL | Options.DisplayControl);
}

void Lcd::CursorOn(void)
{
	Options.DisplayControl |= LCD_CURSORON;
	Cmd(LCD_DISPLAYCONTROL | Options.DisplayControl);
}

void Lcd::CursorOff(void)
{
	Options.DisplayControl &= ~LCD_CURSORON;
	Cmd(LCD_DISPLAYCONTROL | Options.DisplayControl);
}

void Lcd::ScrollLeft(void)
{
	Cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
}

void Lcd::ScrollRight(void)
{
	Cmd(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
}

void Lcd::CreateChar(uint8_t location, uint8_t const *data)
{
	uint8_t i;
	/* We have 8 locations available for custom characters */
	location &= 0x07;
	Cmd(LCD_SETCGRAMADDR | (location << 3));
	
	for (i = 0; i < 8; i++) {
		Data(data[i]);
	}
}

/*void Read_str(char* str,uint8_t x, uint8_t y,uint8_t length)
{
	CursorSet(x, y);

	while (length)
  {
		if (Options.currentX >= _LCD_COLS)
    {
			Options.currentX = 0;
			Options.currentY++;
			CursorSet(Options.currentX, Options.currentY);
		}
		else
    {
			*str=(char)LCD_Read_char();
			Options.currentX++;
		}
		str++;
		length--;
	}
}*/

void Lcd::PutCustom(uint8_t x, uint8_t y, uint8_t location)
{
	CursorSet(x, y);
	Data(location);
}

/*static*/ void Lcd::Cmd(uint8_t cmd)
{
	RstLow();
	Cmd4bit(cmd >> 4);
	Cmd4bit(cmd & 0x0F);
}

/*static*/ void Lcd::Data(uint8_t data)
{
	RstHigh();
	Cmd4bit(data >> 4);
	Cmd4bit(data & 0x0F);
}

/*static uint8_t LCD_Read_char(void)
{
	setpin_in();
	RS_HIGH;

	uint8_t data=Data_read()<<4;
	data|=Data_read();

	setpin_out();
	return data;
}
//############################################################################################
static uint8_t Data_read(void)
{
	E_HIGH;
	Delay_us(50);

	uint8_t data =HAL_GPIO_ReadPin(_LCD_D7_PORT, _LCD_D7_PIN)<<3;
	data|=HAL_GPIO_ReadPin(_LCD_D6_PORT, _LCD_D6_PIN)<<2;
	data|=HAL_GPIO_ReadPin(_LCD_D5_PORT, _LCD_D5_PIN)<<1;
	data|=HAL_GPIO_ReadPin(_LCD_D4_PORT, _LCD_D4_PIN)<<0;

	E_LOW;
	Delay_us(50);

	return data;
}
//############################################################################################
static void setpin_in(void)
{
	RW_HIGH;

  GPIO_InitTypeDef  gpio;
  gpio.Mode = GPIO_MODE_INPUT;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio.Pull = GPIO_NOPULL;

  gpio.Pin = _LCD_D4_PIN;
  HAL_GPIO_Init(_LCD_D4_PORT,&gpio);
  gpio.Pin = _LCD_D5_PIN;
  HAL_GPIO_Init(_LCD_D5_PORT,&gpio);
  gpio.Pin = _LCD_D6_PIN;
  HAL_GPIO_Init(_LCD_D6_PORT,&gpio);
  gpio.Pin = _LCD_D7_PIN;
  HAL_GPIO_Init(_LCD_D7_PORT,&gpio);
}
//############################################################################################
static void setpin_out(void)
{
	RW_LOW;

  GPIO_InitTypeDef  gpio;
  gpio.Mode = GPIO_MODE_OUTPUT_PP;
  gpio.Speed = GPIO_SPEED_FREQ_HIGH;
  gpio.Pull = GPIO_NOPULL;

  gpio.Pin = _LCD_D4_PIN;
  HAL_GPIO_Init(_LCD_D4_PORT,&gpio);
  gpio.Pin = _LCD_D5_PIN;
  HAL_GPIO_Init(_LCD_D5_PORT,&gpio);
  gpio.Pin = _LCD_D6_PIN;
  HAL_GPIO_Init(_LCD_D6_PORT,&gpio);
  gpio.Pin = _LCD_D7_PIN;
  HAL_GPIO_Init(_LCD_D7_PORT,&gpio);
}*/

/*static*/ void Lcd::Cmd4bit(uint8_t cmd)
{
	hal.WritePin(LCD7_GPIO_Port, LCD7_Pin, (GPIO_PinState)(cmd & 0x08));
	hal.WritePin(LCD6_GPIO_Port, LCD6_Pin, (GPIO_PinState)(cmd & 0x04));
	hal.WritePin(LCD5_GPIO_Port, LCD5_Pin, (GPIO_PinState)(cmd & 0x02));
	hal.WritePin(LCD4_GPIO_Port, LCD4_Pin, (GPIO_PinState)(cmd & 0x01));
	EnBlink();
}

/*static*/ void Lcd::CursorSet(uint8_t col, uint8_t row)
{
	uint8_t row_offsets[] = {0x00, 0x40, 0x14, 0x54};
	if (row >= _LCD_ROWS)
		row = 0;
	Options.currentX = col;
	Options.currentY = row;
	Cmd(LCD_SETDDRAMADDR | (col + row_offsets[row]));
}

void Lcd::Put(uint8_t data)
{
	Data(data);	
}

// It draws lines in lcd.
void Lcd::HLine(char x, char y, uint8_t KindofLine, char mem8)
{
	CreateChar(mem8, Line[KindofLine]);
	CursorSet(x, y);
	Data(mem8);
}

void Lcd::drawBitmap(char x, char y, const uint8_t *image, char mem8)
{
	CreateChar(mem8, image);
	CursorSet(x, y);
	Data(mem8);
}