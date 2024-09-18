#ifndef __PWM_H_
#define __PWM_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "main.h"
#include "math.h"	

#define Freq_CLK    72000000          //72 MHz
#define PWM_PSC     7200-1            //7200
#define PWM_CLK     Freq_CLK/(PWM_PSC+1)  //72 KHz
	
void SetFrequency(float frequency);
void SetDutyCycle(float Ton);
	 	 

#ifdef __cplusplus
}
#endif

#endif  /* __PWM_H */





