/*
 * sntp_rtc.c
 *
 *  Created on: Nov 12, 2022
 *      Author: anton_000
 */
#include "sntp_rtc.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
time_t sntp_time;
void SetRTC(uint64_t seconds,uint64_t us)
{
	RTC_TimeTypeDef rtc_time;
	RTC_DateTypeDef rtc_date;
	struct tm sntp_clock;
	sntp_time=seconds;
	localtime_r(&sntp_time,&sntp_clock);

	rtc_time.Hours=sntp_clock.tm_hour;
	rtc_time.Minutes=sntp_clock.tm_min;
	rtc_time.Seconds=sntp_clock.tm_sec;
	rtc_time.DayLightSaving=(sntp_clock.tm_isdst)? RTC_DAYLIGHTSAVING_ADD1H : RTC_DAYLIGHTSAVING_NONE;

	rtc_date.Date=sntp_clock.tm_mday;
	rtc_date.Month=sntp_clock.tm_mon+1;
	rtc_date.Year=sntp_clock.tm_year%100;
	rtc_date.WeekDay=sntp_clock.tm_wday+1;

	HAL_RTC_SetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc, &rtc_date, RTC_FORMAT_BIN);

	HAL_RTC_GetTime(&hrtc, &rtc_time, RTC_FORMAT_BIN);
	uint32_t us_rtc = (rtc_time.SecondFraction-rtc_time.SubSeconds)/((float)rtc_time.SecondFraction+1)*1000000;
	int us_offset=us-us_rtc;
	if(us_offset<0)
	{	HAL_RTCEx_SetSynchroShift(&hrtc,RTC_SHIFTADD1S_RESET,(256*abs(us_offset)/1000000));
	}else
	{
		HAL_RTCEx_SetSynchroShift(&hrtc,RTC_SHIFTADD1S_SET,256-(256*us_offset/1000000));
	}
}

