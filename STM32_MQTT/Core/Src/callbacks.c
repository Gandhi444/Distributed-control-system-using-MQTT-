/*
 * Callbacks.h
 *
 *  Created on: Nov 27, 2022
 *      Author: anton_000
 */
#include "tim.h"
#include "main.h"
#include "rtc.h"
#include "usart.h"
#include "BH1750.h"
#include "BMP280.h"
#include "lwip/apps/mqtt.h"
#include "lwip_mqtt.h"
extern mqtt_client_t *client;
extern BMP280_typedef BMP280;
#include <time.h>
extern uint8_t mode;
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef * htim)
{
	if(htim->Instance==TIM2)
	{
		if(mode==1)
		{
		char buf[1000];
		uint16_t len=0;
		RTC_TimeTypeDef RTC_time;
		RTC_DateTypeDef RTC_date;
		BMP280ReadTemp(&BMP280);
		HAL_RTC_GetTime(&hrtc, &RTC_time, RTC_FORMAT_BIN);
		HAL_RTC_GetDate(&hrtc, &RTC_date, RTC_FORMAT_BIN);
		float miliseconds = (RTC_time.SecondFraction-RTC_time.SubSeconds)/((float)RTC_time.SecondFraction+1);
		struct tm brokentime = { RTC_time.Seconds, RTC_time.Minutes,RTC_time.Hours, RTC_date.Date, RTC_date.Month-1, RTC_date.Year+100 };
		time_t rawtime = mktime(&brokentime);


		//len=sprintf(buf,"h:%d,m:%d,s:%d ms:%f temp=%f\n\r",RTC_time.Hours,RTC_time.Minutes,RTC_time.Seconds,miliseconds,BMP280.temp);
		len=sprintf(buf,"{\"s\":%ld, \"ms\":%f, \"temp\":%f} \n", rawtime,miliseconds,BMP280.temp);
		//example_do_connect(client, "hello_world");
		example_publish(client,"Sensors", buf);
		//HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
		}
		else
		{

		}
	}
}
