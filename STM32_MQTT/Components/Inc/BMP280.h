/*
 * BMP280.h
 *
 *  Created on: 22 lis 2022
 *      Author: anton
 */
#include "main.h"

#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#define BMP280_ADRESS (0x76 << 1)

#define BMP280_MODE_NORMAL  3
#define BMP280_FILTER_OFF 0
#define BMP280_HIGH_RES  4
#define BMP280_STANDBY_250  3




typedef struct {
	float temp;
    uint16_t mode;
    uint16_t adress;
    uint16_t filter;
    uint16_t oversampling_temperature;
    uint16_t standby;
} BMP180_typedef;

void BMP280_init(BMP180_typedef* BMP280);


#endif /* INC_BMP280_H_ */
