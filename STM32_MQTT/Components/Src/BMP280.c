/*
 * BMP280.c
 *
 *  Created on: Nov 22, 2022
 *      Author: anton
 */

#include "BMP280.h"
bool BMP280_init(BMP280_typedef* BMP280)
{
	BMP280->hi2c=&hi2c1;
	BMP280->address=BMP280_ADRESS;
	uint8_t buf = (BMP280_STANDBY_250 << 5) | (BMP280_FILTER_2 << 2);
	if(HAL_I2C_Mem_Write(BMP280->hi2c, BMP280->address, CONFIG_REG, 1, &buf, 1, 5000) != HAL_OK)
	{
		return false;
	}
	buf=(BMP280_TEMP_OVERSAMPLING << 5)| (BMP280_PRE_OVERSAMPLING << 2) | (BMP280_MODE_NORMAL);
	if(HAL_I2C_Mem_Write(BMP280->hi2c, BMP280->address, CTRL_REG, 1, &buf, 1, 5000) != HAL_OK)
	{
		return false;
	}
	BMP280ReadTempCompensation(BMP280);
	return true;
}
bool BMP280ReadTempCompensation(BMP280_typedef* BMP280)
{
	uint8_t buf[6];
	if (HAL_I2C_Mem_Read(BMP280->hi2c, BMP280->address, TEMP_COMP, 1, buf, 6, 5000) == HAL_OK)
	{
		BMP280->dig_T1=(uint16_t) ((buf[1] << 8) | buf[0]);
		BMP280->dig_T2=(int16_t) ((buf[3] << 8) | buf[2]);
		BMP280->dig_T3=(int16_t) ((buf[5] << 8) | buf[4]);
		return true;
	}
	return false;
}
bool BMP280ReadTemp(BMP280_typedef* BMP280)
{
	uint8_t buf[3];
	int32_t raw_temp;
	if (HAL_I2C_Mem_Read(BMP280->hi2c, BMP280->address, TEMP_MSB, 1, buf, 3, 5000) == HAL_OK)
	{


		int32_t var1, var2;
		raw_temp = buf[0] << 12 | buf[1] << 4 | buf[2] >> 4;
		var1 = ((((raw_temp >> 3) - ((int32_t) BMP280->dig_T1 << 1)))* (int32_t) BMP280->dig_T2) >> 11;
		var2 = (((((raw_temp >> 4) - (int32_t) BMP280->dig_T1)* ((raw_temp >> 4) - (int32_t) BMP280->dig_T1)) >> 12) * (int32_t) BMP280->dig_T3) >> 14;
		BMP280->temp=(((var1+var2)* 5 + 128)>>8)/100.0;
		return true;
	}
	return false;
}
