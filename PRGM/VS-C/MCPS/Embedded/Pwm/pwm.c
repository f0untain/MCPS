#include "pwm.h"

float Frequency;

void SetFrequency(float frequency)
{
	/* Frequency_PWM = Frequency_CLK/((APRx+1)*(PSCx+1))
	 * Frequency_PWM = It is the input of function that user want to set
	 * Frequency_CLK = 72 MHz
	 * APRx = Auto-reload register
	 * PSCx = The value of prescaler */
	volatile static uint32_t PWM_APR; //start PWM-Frequency with 100Hz
	PWM_APR = (PWM_CLK / frequency) - 1;
	TIM3->ARR = PWM_APR;
}

void SetDutyCycle(float Ton)
{
	/* DutyCycle = CCRx/APRx = Ton/(Ton + Toff)
	 * CCRx = The value for compare */
	float dutycycle = (Ton / 1000.000) * (Frequency / 1.000);

	//assert(((dutycycle * 100) >= 0.0F) && ((dutycycle * 100) <= 100.0F));

	uint32_t newRegVal = (uint32_t)roundf((float)(TIM3->ARR + 1) * dutycycle);

	//In case of the dutycycle being calculated as higher than the reload register, cap it to the reload register
	if (newRegVal > TIM3->ARR)
		newRegVal = TIM3->ARR;

	TIM3->CCR1 = (uint32_t)(roundf(newRegVal));
	//__HAL_TIM_SET_COMPARE(&htim3, TIM_CHANNEL_1, TIM3->CCR1);
}

