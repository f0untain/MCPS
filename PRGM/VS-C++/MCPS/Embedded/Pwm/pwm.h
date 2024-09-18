#ifndef __PWM_H_
#define __PWM_H_

#ifdef __cplusplus
extern "C" {
#endif
	
#include "stm32f3xx_hal.h"
#include "math.h"	

#define Freq_CLK    72000000          //72 MHz
#define PWM_PSC     7200-1            //7200
#define PWM_CLK     Freq_CLK/(PWM_PSC+1)  //72 KHz
	
class Pwm
{
	public:
	
	float VariableRamp = 0.1;
	float StepRamp = 0.0;
	uint16_t CountRamp = 0;
	float Time_ON;
	float Frequency;
	int RythmOn;
	int RythmOff;
	
	Pwm();
	~Pwm();
	void SetFrequency(float frequency);
	void SetDutyCycle(float Ton);
	
	private:
	

	
};
	 	 

#ifdef __cplusplus
}
#endif

#endif  /* __PWM_H */





