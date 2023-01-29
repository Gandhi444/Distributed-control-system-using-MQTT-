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
#include"PID.h"

extern BMP280_typedef BMP280;
extern BH1750_typedef BH1750;
extern PID_typedef PID;
extern TIM_HandleTypeDef htim3;
#include <time.h>
extern enum mode_enum MODE;
int i = 0;
float miliseconds;
time_t rawtime;
double combined_time;
float duty=0;
float SetpointLight=100;
float SetpointTemp=21;
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	//HAL_GPIO_WritePin(Osciloscope_GPIO_Port, Osciloscope_Pin, GPIO_PIN_SET);
	uint16_t len = 0;
	char buf[1000];
	if (htim->Instance == TIM2) {
		switch (MODE) {
		case Temp: {

			BMP280ReadTemp(&BMP280);
			GetRTC(&rawtime, &miliseconds);
			duty=PIDCall(&PID, SetpointTemp, BMP280.temp);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,__HAL_TIM_GET_AUTORELOAD(&htim3)*duty/100);
			combined_time=rawtime+miliseconds;
			len = sprintf(buf, "{\"s\":%d,\"ms\":%f, \"temp\":%.2f,\"u\":%f} \n\r",rawtime,miliseconds, BMP280.temp,duty);
			HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
			break;
		}
		case Light: {
			BH1750_ReadIlluminance_lux(&BH1750);
			GetRTC(&rawtime, &miliseconds);
			duty=PIDCall(&PID, SetpointLight, BH1750.Iluminance);
			__HAL_TIM_SET_COMPARE(&htim3,TIM_CHANNEL_1,__HAL_TIM_GET_AUTORELOAD(&htim3)*duty/100);
			combined_time=rawtime+miliseconds;
			len = sprintf(buf, "{\"s\":%d,\"ms\":%f, \"lux\":%.2f,\"u\":%f} \n\r",rawtime,miliseconds, BH1750.Iluminance,duty);
			HAL_UART_Transmit(&huart3, (uint8_t*) buf, len, 1000);
			break;
		}

		}
	}
	//HAL_GPIO_WritePin(Osciloscope_GPIO_Port, Osciloscope_Pin, GPIO_PIN_RESET);
}
