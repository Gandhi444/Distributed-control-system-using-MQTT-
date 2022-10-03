/*
 * MLX90614.c
 *
 *  Created on: 1 paź 2022
 *      Author: anton_000
 */

#include "MLX90614.h"

bool MLX90614_Init(MLX90614_typedef *MLX90614, I2C_HandleTypeDef *hi2c) {
	MLX90614->addres = MLX90614_BASE_ADRESS;
	MLX90614->hi2c = hi2c;
	return true;
}
bool MLX90614_Read(uint8_t Register, uint16_t *Result,MLX90614_typedef *MLX90614) {

	uint8_t tmp[2];

	if (HAL_OK!= HAL_I2C_Mem_Read(MLX90614->hi2c, MLX90614->addres,Register, 1, tmp, 2, 10))
	{
		return false;
	}
	*Result = (uint16_t) tmp[1] << 8 | tmp[0];

	return true;
}
bool MLX90614_ReadEprom(uint8_t Register, uint16_t *Result,MLX90614_typedef *MLX90614)
{
	uint8_t tmp[2];
	Register &= 0x1F;
	Register |= 0x20;
	if (HAL_OK!= HAL_I2C_Mem_Read(MLX90614->hi2c, MLX90614->addres,Register, 1, tmp, 2, 10))
	{
		return false;
	}
	*Result = (uint16_t) tmp[1] << 8 | tmp[0];

	return true;
}


bool MLX90614_Read_Temp(uint16_t *Result,MLX90614_typedef *MLX90614){
	uint16_t tmp;

	if(!MLX90614_Read(MLX90614_TOBJ, &tmp,MLX90614))
	{
		return false;
	}

	MLX90614->temp = (float)tmp* 0.02 - 273.15;

	return true;

}

bool MLX90614_Amb_Temp(uint16_t *Result,MLX90614_typedef *MLX90614){
	uint16_t tmp;

	if(!MLX90614_Read(MLX90614_TA, &tmp,MLX90614))
	{
		return false;
	}

	MLX90614->amb_temp = (float)tmp* 0.02 - 273.15;

	return true;

}
