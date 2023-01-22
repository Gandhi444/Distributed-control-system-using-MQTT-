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
#include "sntp_rtc.h"

extern BMP280_typedef BMP280;
extern BH1750_typedef BH1750;
#include <time.h>
extern enum mode_enum MODE;
int i = 0;
float miliseconds;
time_t rawtime;
float combined_time;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	//HAL_GPIO_WritePin(Osciloscope_GPIO_Port, Osciloscope_Pin, GPIO_PIN_SET);
	uint16_t len = 0;
	char buf[1000];
	if (htim->Instance == TIM2) {
		switch (MODE) {
		case Temp: {

			BMP280ReadTemp(&BMP280);


			GetRTC(&rawtime, &miliseconds);
			combined_time=rawtime+miliseconds;
			len = sprintf(buf, "{\"time\":%f, \"temp\":%.2f} \n",combined_time, BMP280.temp);
			HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
			break;
		}
		case Light: {
			BH1750_ReadIlluminance_lux(&BH1750);
			GetRTC(&rawtime, &miliseconds);
			combined_time=rawtime+miliseconds;
			len = sprintf(buf, "{\"time\":%f, \"lux\":%.2f} \n",combined_time, BH1750.Iluminance);
			HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
			break;
		}

		}
	}
	//HAL_GPIO_WritePin(Osciloscope_GPIO_Port, Osciloscope_Pin, GPIO_PIN_RESET);
}
