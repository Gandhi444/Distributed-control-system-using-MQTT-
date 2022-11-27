/*
 * BMP280.c
 *
 *  Created on: Nov 22, 2022
 *      Author: anton
 */

#include "BMP280.h"

void BMP280_initDefParams(BMP280_typedef* BMP280)
{
	BMP280->address=BMP280_ADRESS;
	BMP280->mode=BMP280_MODE_NORMAL;
	BMP280->filter=BMP280_FILTER_OFF;
	BMP280->oversampling=BMP280_HIGH_RES;
	BMP280->standby=BMP280_STANDBY_250;
	BMP280->hi2c=&hi2c1;
	return;
}
bool write_register8(BMP280_typedef *dev, uint8_t addr, uint8_t value) {
	uint16_t buf;

	buf = dev->address;

	if (HAL_I2C_Mem_Write(dev->hi2c, buf, addr, 1, &value, 1, 5000) == HAL_OK)
		return true;
	else
		return false;
}


bool BMP280_init(BMP280_typedef* BMP280)
{	uint8_t buf = (BMP280->standby << 5) | (BMP280->filter << 2);
	if(!write_register8(BMP280,CONFIG_REG,buf))
	{
		return false;
	}
	buf=(BMP280->oversampling << 5)| (BMP280->oversampling << 2) | (BMP280->mode);
	if(!write_register8(BMP280,CTRL_REG,buf))
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
