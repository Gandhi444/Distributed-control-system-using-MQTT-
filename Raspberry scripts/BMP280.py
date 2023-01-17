from smbus2 import SMBus
import numpy as np
BMP280_ADRESS = (0x76)
BMP280_MODE_NORMAL = 3
BMP280_FILTER_OFF = 0
BMP280_HIGH_RES = 4
BMP280_STANDBY_250 = 3
CTRL_REG = 0xF4
CONFIG_REG = 0xF5
TEMP_MSB = 0xFA
TEMP_COMP = 0x88


class BMP280():
    dig_T1=None
    dig_T2=None
    dig_T3=None
    temp=None
    i2c=None
    def __init__(self, i2c_handle):
        self.addres = BMP280_ADRESS
        self.mode = BMP280_MODE_NORMAL
        self.filter = BMP280_FILTER_OFF
        self.oversamling = BMP280_HIGH_RES
        self.standby = BMP280_STANDBY_250
        self.i2c = i2c_handle
        buf = (self.standby << 5) | (self.filter << 2)
        self.i2c.write_byte_data(self.addres,CONFIG_REG,buf)
        buf=(self.oversamling << 5) | (self.oversamling << 2) | ( self.mode)
        self.i2c.write_byte_data(self.addres,CTRL_REG,buf)
        self.ReadTempCompensation()
    def ReadTempCompensation(self):
        buf=self.i2c.read_i2c_block_data(self.addres,TEMP_COMP,6)
        self.dig_T1=np.uint16((buf[1] << 8) | buf[0])
        self.dig_T2=np.int16((buf[3] << 8) | buf[2])
        self.dig_T3=np.int16((buf[5] << 8) | buf[4])
    def ReadTemp(self):
        buf=self.i2c.read_i2c_block_data(self.addres,TEMP_MSB,3)
        var1=np.int32
        var2=np.int32
        raw_temp = np.int32(buf[0] << 12 | buf[1] << 4 | buf[2] >> 4)
        var1=((raw_temp >> 3)-np.int32(self.dig_T1 << 1))*np.int32(self.dig_T2 >> 11)
        var2=((raw_temp >> 4)-np.int32(self.dig_T1))*((raw_temp>>4) -np.int32(self.dig_T1 >> 12))*np.int32(self.dig_T3 >> 14)
        self.temp=(((var1+var2)*5+128)>>8)/100.0