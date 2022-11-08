/*
 * BH1750.c
 *
 *  Created on: 25 paź 2022
 *      Author: anton
 */
#include "BH1750.h"

bool BH1750_init(BH1750_typedef *BH1750) {
	uint8_t command;
	command = 0x01;
	if (HAL_I2C_Master_Transmit(BH1750->hi2c, BH1750->Address, &command, 1, 1000)!= HAL_OK)
	{
		return false;
	}
	command = 0x10;
	if (HAL_I2C_Master_Transmit(BH1750->hi2c, BH1750->Address, &command, 1, 1000)!= HAL_OK)
	{
		return false;
	}
	return true;
}
bool BH1750_ReadIlluminance_lux(BH1750_typedef *BH1750) {
	uint8_t read_buf[2];
	if (HAL_I2C_Master_Receive(BH1750->hi2c, BH1750->Address, read_buf, 2, 1000)== HAL_OK)
	{
		BH1750->Iluminance=(read_buf[0]<<8 | read_buf[1])/1.2f;
		return true;
	}
	return false;
}
