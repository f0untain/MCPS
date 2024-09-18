#ifndef __HAL_H_
#define __HAL_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f3xx_hal.h"	
#include "main.h"	

#define BUZZ_NUM	0
#define BUZZ_DELAY	1
	
	
class Hal
{
	public:

	Hal();
	~Hal();
	void WritePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin, GPIO_PinState PinState);
	void ReadPin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void TogglePin(GPIO_TypeDef* GPIOx, uint16_t GPIO_Pin);
	void ToggleBuzzer(volatile uint8_t buzzer[2]);
	void I2cMemWrite(I2C_HandleTypeDef *hi2c,
		uint16_t DevAddress,
		uint16_t MemAddress,
		uint16_t MemAddSize,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout);
	void I2cMemRead(I2C_HandleTypeDef *hi2c,
		uint16_t DevAddress,
		uint16_t MemAddress,
		uint16_t MemAddSize,
		uint8_t *pData,
		uint16_t Size,
		uint32_t Timeout);
	
	private:
	
};
	 	 

#ifdef __cplusplus
}
#endif

#endif  /* __HAL_H_ */





