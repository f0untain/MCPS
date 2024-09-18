

#include "clockconfig.h"


Clock* Clock::mp_Clock = NULL;

Clock* Clock::GetInstance()
{
	if (mp_Clock == NULL)
	{
		mp_Clock = new Clock();
	}
	return mp_Clock;
}

Clock::Clock()
{
	initializeClock();
}

Clock::~Clock()
{
	
}

void Clock::initializeClock()
{

	MODIFY_REG(FLASH->ACR, FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_1); /* Flash Latency config  */  
	
	SET_BIT(RCC->CR, RCC_CR_HSEON);/* HSE ON  */ 
	while(!(READ_BIT(RCC->CR, RCC_CR_HSERDY) == (RCC_CR_HSERDY))); /* Wait for HSE ready  */ 
	
	
	
	//	SET_BIT(RCC->CSR, RCC_CSR_LSION); /* LSI ON  */
	//	while (!(READ_BIT(RCC->CSR, RCC_CSR_LSIRDY) == (RCC_CSR_LSIRDY))) ;/* Wait for LSI ready  */
	SET_BIT(RCC->APB1ENR, RCC_APB1ENR_PWREN);
//	SET_BIT(PWR->CR, PWR_CR_DBP); 
//    SET_BIT(RCC->BDCR, RCC_BDCR_LSEON); /* LSE ON  */
//	while (!(READ_BIT(RCC->BDCR, RCC_BDCR_LSERDY) == (RCC_BDCR_LSERDY))) ;/* Wait for LSE ready  */
	CLEAR_BIT(PWR->CR, PWR_CR_DBP); 
	/* PLL Config  */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SWS_HSI);/* System clock source change for RCC_CFGR_PLLSRC Config  */
	CLEAR_BIT(RCC->CR, RCC_CR_PLLON);/* PLL Disable  */
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PLLSRC | RCC_CFGR_PLLMUL, ((RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR2_PREDIV_DIV1) & RCC_CFGR_PLLSRC) | RCC_CFGR_PLLMUL9);
	MODIFY_REG(RCC->CFGR2, RCC_CFGR2_PREDIV, ((RCC_CFGR_PLLSRC_HSE_PREDIV | RCC_CFGR2_PREDIV_DIV1) & RCC_CFGR2_PREDIV));

	SET_BIT(RCC->CR, RCC_CR_PLLON);/* PLL Enable  */
	
	while (!(READ_BIT(RCC->CR, RCC_CR_PLLRDY) == (RCC_CR_PLLRDY)));/* Wait for PLL ready  */
	
	MODIFY_REG(RCC->CFGR, RCC_CFGR_SW, RCC_CFGR_SW_PLL); /* System clock source PLL  */
	while (!(READ_BIT(RCC->CFGR, RCC_CFGR_SWS))) ;/* Wait for System clock switch status ready  */
	
	MODIFY_REG(RCC->CFGR, RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);
	
	MODIFY_REG(RCC->CFGR, RCC_CFGR_PPRE2, RCC_CFGR_PPRE1_DIV1);	
}