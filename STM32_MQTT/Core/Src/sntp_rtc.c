/*
 * sntp_rtc.c
 *
 *  Created on: Nov 12, 2022
 *      Author: anton_000
 */
#include "sntp_rtc.h"
#include <time.h>
#include <stdio.h>
time_t sntp_time;
void SetRTC(uint64_t* seconds,uint64_t* us)
{
	RTC_TimeTypeDef rtc_time;
	struct tm sntp_clock;
	sntp_time=seconds;
	localtime_r(&sntp_time,&sntp_clock);
	rtc_time.Hours=sntp_clock.tm_hour;
	rtc_time.Minutes=sntp_clock.tm_min;
	rtc_time.Seconds=sntp_clock.tm_sec;
	//rtc_time.SubSeconds=us;
	//rtc_time.SecondFraction=999999;
	HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
}

