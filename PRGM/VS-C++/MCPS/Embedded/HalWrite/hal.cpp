#include "hal.h"

Hal::Hal()
{
	
}
Hal::~Hal()
{
	
}
void Hal::WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState)
{
	HAL_GPIO_WritePin(GPIOx, GPIO_Pin, PinState);
}
void Hal::ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_ReadPin(GPIOx, GPIO_Pin);
}
void Hal::TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin)
{
	HAL_GPIO_TogglePin(GPIOx, GPIO_Pin);
}
void Hal::ToggleBuzzer(volatile uint8_t buzzer[2])
{
	static uint8_t number = 0;
	static uint8_t count10ms = 0;

	if (count10ms == 0) /* buzzer[BUZZ_DELAY]/10 = number of count10ms */
	{
		if (number < buzzer[BUZZ_NUM] * 2)	/* buzzer[BUZZ_NUM]*2 = number of (buzzer_off + buzzer_on) BUT
											   buzzer[BUZZ_NUM]	  = number of buzzer_on */
		{
			//BUZZ_TGL;
			TogglePin(Buzzer_GPIO_Port, Buzzer_Pin);
			number++;
			count10ms = (buzzer[BUZZ_DELAY] / 10);
		}
		else
		{
			WritePin(Buzzer_GPIO_Port, Buzzer_Pin, GPIO_PIN_RESET);
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

void Hal::I2cMemWrite(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
	uint16_t MemAddress, uint16_t MemAddSize,
	uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	HAL_I2C_Mem_Write(hi2c, DevAddress,
		MemAddress, MemAddSize,
		pData, Size, Timeout);
}

void Hal::I2cMemRead(I2C_HandleTypeDef *hi2c, uint16_t DevAddress,
	uint16_t MemAddress, uint16_t MemAddSize,
	uint8_t *pData, uint16_t Size, uint32_t Timeout)
{
	HAL_I2C_Mem_Read(hi2c, DevAddress,
        MemAddress, MemAddSize,
		pData, Size, Timeout);
}
