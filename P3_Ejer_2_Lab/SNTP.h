#ifndef __SNTP_H
#define __SNTP_H

	#include "stm32f4xx_hal.h"
	#include "cmsis_os2.h"                          // CMSIS RTOS header file
	#include <stdio.h>
	#include <string.h>
	#include "rl_net.h"
	#include "lcd.h"
	#include <time.h>
	#include "rtc.h"
	
	int Init_ThSNTP(void);
	int Init_timer_SNTP(void) ;
	
	void get_SNTP_Time(void);
	void getLocalTime(uint32_t seconds);
	

#endif
