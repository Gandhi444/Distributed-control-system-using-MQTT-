/*
 * BMP280.h
 *
 *  Created on: 22 lis 2022
 *      Author: anton
 */
#include "main.h"
#include "stdbool.h"
#ifndef INC_BMP280_H_
#define INC_BMP280_H_

#define BMP280_ADRESS (0x76 << 1)

#define BMP280_MODE_NORMAL  3
#define BMP280_FILTER_OFF 0
#define BMP280_HIGH_RES  4
#define BMP280_STANDBY_250  3
#define CTRL_REG 0xF4
#define CONFIG_REG 0xF5
#define TEMP_MSB 0xFA
#define TEMP_COMP 0x88


extern I2C_HandleTypeDef hi2c1;


typedef struct {
	float temp;
    uint16_t mode;
    uint16_t address;
    uint16_t filter;
    uint16_t oversampling;
    uint16_t standby;
    I2C_HandleTypeDef* hi2c;
    uint16_t dig_T1;
    int16_t dig_T2;
    int16_t dig_T3;
} BMP280_typedef;

void BMP280_initDefParams(BMP280_typedef* BMP280);
bool write_register8(BMP280_typedef *dev, uint8_t addr, uint8_t value);
bool BMP280_init(BMP280_typedef* BMP280);
bool BMP280ReadTempCompensation(BMP280_typedef* BMP280);
bool BMP280ReadTemp(BMP280_typedef* BMP280);
#endif /* INC_BMP280_H_ */
