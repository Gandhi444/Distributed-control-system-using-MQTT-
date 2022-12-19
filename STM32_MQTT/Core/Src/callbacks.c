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
extern BH1750_typedef BH1750;
#include <time.h>
extern enum mode_enum MODE;
int i = 0;
float miliseconds;
time_t rawtime;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	uint16_t len = 0;
	char buf[1000];
	if (htim->Instance == TIM2) {
		switch (MODE) {
		case Temp: {
			BMP280ReadTemp(&BMP280);
			GetRTC(&rawtime, &miliseconds);
			len = sprintf(buf, "{\"s\":%lld, \"ms\":%f, \"temp\":%f} \n",
					rawtime, miliseconds, BMP280.temp);
			if (mqtt_client_is_connected(client)) {
				example_publish(client, "Sensors", buf);
			}
			//len=sprintf(buf,"h:%d,m:%d,s:%d ms:%f temp=%f\n\r",RTC_time.Hours,RTC_time.Minutes,RTC_time.Seconds,miliseconds,BMP280.temp);
			//HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
			break;
		}
		case Light: {
			BH1750_ReadIlluminance_lux(&BH1750);
			GetRTC(&rawtime, &miliseconds);
			len = sprintf(buf, "{\"s\":%lld, \"ms\":%f, \"Luminance\":%f} \n",
					rawtime, miliseconds, BH1750.Iluminance);
			if (mqtt_client_is_connected(client)) {
				example_publish(client, "Sensors", buf);
			}
			//len=sprintf(buf,"h:%d,m:%d,s:%d ms:%f temp=%f\n\r",RTC_time.Hours,RTC_time.Minutes,RTC_time.Seconds,miliseconds,BMP280.temp);
			//HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
			break;
		}
		case Efector: {
			if (i == 100) {
				int compare = __HAL_TIM_GET_COMPARE(&htim2, TIM_CHANNEL_3);
				len = sprintf(buf, "compare: %d \n\r", compare);
				HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
				i = 0;
			}
			i++;
			break;
		}
		case Not_selected:
		{
			if (i == 100) {
			len = sprintf(buf, "somthing went wrong\n\r");
			HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
			}
		}

		}
	}
}
