/*
 * BH1750.h
 *
 *  Created on: 25 paź 2022
 *      Author: anton
 */

#ifndef INC_BH1750_H_
#define INC_BH1750_H_

#include <stdio.h>
#include "stdbool.h"
#include "math.h"
#include "main.h"

typedef struct{
	uint8_t Address;
	float Iluminance;
	I2C_HandleTypeDef *hi2c;
}BH1750_typedef;

bool BH1750_init(BH1750_typedef* BH1750);
bool BH1750_ReadIlluminance_lux(BH1750_typedef* BH1750);


#endif /* INC_BH1750_H_ */
