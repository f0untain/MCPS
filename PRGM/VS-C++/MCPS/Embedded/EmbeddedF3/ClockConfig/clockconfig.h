
#ifndef __CLOCKCONFIG_H
#define __CLOCKCONFIG_H

#ifdef __cplusplus
 extern "C" {
#endif

#include "stm32f3xx.h"
#include "stdint.h"
#include "stddef.h"	 
	 
 class Clock
{
 public:
	 static Clock* GetInstance();
 private:
	 static Clock*	mp_Clock;	
	Clock();
	~Clock();
	void initializeClock();
 
 };
	 

	 #ifdef __cplusplus
	 }
#endif
#endif