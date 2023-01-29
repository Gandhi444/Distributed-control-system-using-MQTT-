/*
 * BH1750.h
 *
 *  Created on: 25 paź 2022
 *      Author: anton
 */

#ifndef INC_BH1750_H_
#define INC_BH1750_H_

#define BH1750_ADRESS (0x23 << 1)
#define BH1750_POWER_ON 0x01
#define BH1750_CONTINOUS_H_RES_MODE   0x11


#include <stdio.h>
#include "stdbool.h"
#include "math.h"
#include "main.h"
extern I2C_HandleTypeDef hi2c1;

typedef struct{
	uint8_t Address;
	float Iluminance;
	I2C_HandleTypeDef *hi2c;
}BH1750_typedef;

bool BH1750_init(BH1750_typedef* BH1750);
bool BH1750_ReadIlluminance_lux(BH1750_typedef* BH1750);


#endif /* INC_BH1750_H_ */
