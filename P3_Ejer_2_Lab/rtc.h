#ifndef __RTC_H
#define __RTC_H

#include "stm32f4xx_hal.h"
#include "stm32f4xx_hal_rtc.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"
#include "Parpadeo.h"
#include "lcd.h"
#include "cmsis_os2.h"
#include "SNTP.h"

	void RTC_Init(void);
	void Get_Time_RTC_Binary(void);
	void Set_RTC_Time(uint8_t hour, uint8_t minute, uint8_t second);
	void Get_Date_RTC(void);
	void Set_RTC_Date(uint8_t year, uint8_t month, uint8_t week, uint8_t date);
	void Set_Alarm(uint8_t hour, uint8_t minute, uint8_t second);

	void Get_String_Date_Time(void);
	void Display_Date_Time(void);
	
#endif
