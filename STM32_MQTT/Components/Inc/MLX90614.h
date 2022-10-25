/*
 * MLX90614.h
 *
 *  Created on: 30 wrz 2022
 *      Author: anton_000
 */

#ifndef INC_MLX90614_H_
#define INC_MLX90614_H_

#include "crc.h"
#include <stdio.h>
#include "stdbool.h"
#include "math.h"
//#include "i2c.h"


#define MLX90614_BASE_ADRESS ((0x5A)<<1)
#define MLX90614_TA 0x06
#define MLX90614_TOBJ 0x07
#define MLX90614_EMISS 0x04
#define MLX90614_EMISS_CALIBRATION 0x0F
typedef struct{
	uint8_t addres;
	float temp;
	float amb_temp;
	uint16_t curr_emis;
	I2C_HandleTypeDef *hi2c;
	CRC_HandleTypeDef *hcrc;
}MLX90614_typedef;



bool MLX90614_Init(MLX90614_typedef *MLX90614, I2C_HandleTypeDef *hi2c,CRC_HandleTypeDef *hcrc);

bool MLX90614_Read(uint8_t Register, uint16_t *Result,MLX90614_typedef *MLX90614);

bool MLX90614_ReadEprom(uint8_t Register, uint16_t *Result,MLX90614_typedef *MLX90614);
bool MLX90614_Write_Eeprom(uint8_t Address, uint16_t Value,MLX90614_typedef *MLX90614);

bool MLX90614_Read_Temp(MLX90614_typedef *MLX90614);
bool MLX90614_Amb_Temp(MLX90614_typedef *MLX90614);
bool MLX90614_Set_Emisivity(float Emissivity,MLX90614_typedef *MLX90614);
#endif /* INC_MLX90614_H_ */
