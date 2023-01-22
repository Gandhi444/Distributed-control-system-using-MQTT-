/*
 * MLX90614.c
 *
 *  Created on: 1 paź 2022
 *      Author: anton_000
 */

#include "MLX90614.h"

bool MLX90614_Init(MLX90614_typedef *MLX90614, I2C_HandleTypeDef *hi2c,CRC_HandleTypeDef *hcrc) {
	MLX90614->addres = MLX90614_BASE_ADRESS;
	MLX90614->hi2c = hi2c;
	MLX90614->hcrc = hcrc;
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
	*Result = (uint16_t)(tmp[1] << 8 | tmp[0]);

	return true;
}


bool MLX90614_Read_Temp(MLX90614_typedef *MLX90614){
	uint16_t tmp;

	if(!MLX90614_Read(MLX90614_TOBJ, &tmp,MLX90614))
	{
		return false;
	}

	MLX90614->temp = (float)tmp* 0.02 - 273.15;

	return true;

}

bool MLX90614_Amb_Temp(MLX90614_typedef *MLX90614){
	uint16_t tmp;

	if(!MLX90614_Read(MLX90614_TA, &tmp,MLX90614))
	{
		return false;
	}

	MLX90614->amb_temp = (float)tmp* 0.02 - 273.15;

	return true;

}
bool MLX90614_Write_Eeprom(uint8_t Register, uint16_t Value,MLX90614_typedef *MLX90614)
{
	Register &= 0x1F;
	Register |= 0x20;
	uint16_t verify;
	uint8_t Crc;
	uint8_t crcbuf[4]={MLX90614->addres,Register,0,0};
	Crc=HAL_CRC_Calculate(MLX90614->hcrc, (uint32_t*)crcbuf, 4);
	uint8_t tmp[3]={0,0,Crc};
	if(HAL_OK != HAL_I2C_Mem_Write(MLX90614->hi2c, MLX90614->addres, Register, 1, tmp, 3, 10))
		{
			return false;
		}
	HAL_Delay(10);
	crcbuf[2]=Value&0xFF;
	crcbuf[3]=Value>>8;
	Crc=HAL_CRC_Calculate(MLX90614->hcrc, (uint32_t*)crcbuf, 4);
	tmp[0]=Value & 0xFF;
	tmp[1]=Value>>8;
	tmp[2]=Crc;
	if(HAL_OK != HAL_I2C_Mem_Write(MLX90614->hi2c, MLX90614->addres, Register, 1, tmp, 3, 10))
		{
			return false;
		}
	HAL_Delay(10);
	MLX90614_ReadEprom(Register, &verify, MLX90614);
	if(verify!=Value)
	{
		return false;
	}
	return true;
}
bool MLX90614_Set_Emisivity(float Emissivity,MLX90614_typedef *MLX90614)
{
	if (Emissivity < 0.1 || Emissivity > 1)
			return false;

		uint16_t current_emissivity;
		uint16_t current_calibration;
		uint16_t new_emissivity;
		uint16_t new_calibration;

		MLX90614_ReadEprom(MLX90614_EMISS, &current_emissivity,MLX90614);
		MLX90614_ReadEprom(MLX90614_EMISS_CALIBRATION, &current_calibration,MLX90614);

		new_emissivity = (uint16_t)round(65535.0 * Emissivity);

		if(new_emissivity == current_emissivity)
		{
			return true;
		}

		new_calibration = (uint16_t)(((float)current_emissivity / new_emissivity) * current_calibration);

		if(!MLX90614_Write_Eeprom(MLX90614_EMISS, new_emissivity,MLX90614))
		{
			return false;
		}

		if(!MLX90614_Write_Eeprom(MLX90614_EMISS_CALIBRATION, new_calibration,MLX90614))
		{
			return false;
		}

		return true;
}







