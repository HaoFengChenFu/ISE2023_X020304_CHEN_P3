#ifndef __LOWPOWER_H
#define __LOWPOWER_H

	#include "stm32f4xx_hal.h"
	#include "stm32f4xx_hal_rtc.h"
	#include "stdint.h"
	#include "stdio.h"
	#include "cmsis_os2.h"
	#include "main.h"
	
	#define RTC_ASYNCH_PREDIV 127
	#define RTC_SYNCH_PREDIV 255
	
	int Init_ThLowPower(void);
	int Init_LowPower_Parpadeo(void);
#endif
