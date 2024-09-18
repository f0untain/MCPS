#ifndef __BUTTONCONFIG_H
#define __BUTTONCONFIG_H 

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f3xx_hal.h"
	
#define _BUTTON_PORTS      GPIOA
#define _BUTTON_PORT       GPIOC

#define _BUTTON_MIDDLE_PORT      GPIOA
#define _BUTTON_MIDDLE_PIN       GPIO_PIN_10

#define _BUTTON_STANDBY_PORT     GPIOA
#define _BUTTON_STANDBY_PIN		 GPIO_PIN_11
	
#define _BUTTON_BACK_PORT        GPIOA
#define _BUTTON_BACK_PIN         GPIO_PIN_12
	
#define _BUTTON_START_PORT       GPIOC
#define _BUTTON_START_PIN        GPIO_PIN_14	


#ifdef __cplusplus
}
#endif

#endif  /* __BUTTONCONFIG_H */

