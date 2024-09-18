#ifndef __LCDCONFIG_H
#define __LCDCONFIG_H 

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f3xx_hal.h"

#define _LCD_USE_FREERTOS 1
#define _LCD_COLS         20
#define _LCD_ROWS         4

#define _LCD_RS_PORT      GPIOA
#define _LCD_RS_PIN       GPIO_PIN_7

#define _LCD_E_PORT       GPIOA
#define _LCD_E_PIN        GPIO_PIN_2
	
/* RW Pin is counnected to GND by hardware */
// #define _LCD_RW_PORT      LCD_RW_GPIO_Port
// #define _LCD_RW_PIN       LCD_RW_Pin

#define _LCD_BKL_PORT     GPIOC
#define _LCD_BKL_PIN      GPIO_PIN_13

#define _LCD_D4_PORT      GPIOC
#define _LCD_D4_PIN		  GPIO_PIN_0

#define _LCD_D5_PORT      GPIOC
#define _LCD_D5_PIN       GPIO_PIN_1

#define _LCD_D6_PORT      GPIOC
#define _LCD_D6_PIN       GPIO_PIN_2

#define _LCD_D7_PORT      GPIOC
#define _LCD_D7_PIN       GPIO_PIN_3

#ifdef __cplusplus
}
#endif

#endif  /* __LCDCONFIG_H */

