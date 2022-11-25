/*
 * BMP280.c
 *
 *  Created on: Nov 22, 2022
 *      Author: anton
 */

#include "BMP280.h"

void BMP280_init(BMP180_typedef* BMP280)
{
	BMP280->adress=BMP280_ADRESS;
	BMP280->mode=BMP280_MODE_NORMAL;
	BMP280->filter=BMP280_FILTER_OFF;
	BMP280->oversampling_temperature=BMP280_HIGH_RES;
	BMP280->standby=BMP280_STANDBY_250;
	return;
}
