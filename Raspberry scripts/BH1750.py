from smbus2 import SMBus ,i2c_msg


BH1750_ADRESS =0x23
BH1750_POWER_ON =0x01
BH1750_CONTINOUS_H_RES_MODE  = 0x10
BH1750_CONTINOUS_L_RES_MODE  = 0x13
class BH1750():
    lux=None
    i2c=None
    def __init__(self, i2c_handle):
        self.addres = BH1750_ADRESS
        self.i2c = i2c_handle
        self.i2c.write_byte(self.addres,BH1750_POWER_ON)
        self.i2c.write_byte(self.addres,BH1750_CONTINOUS_L_RES_MODE)
    def ReadLux(self):
        data=self.i2c.read_word_data(self.addres,BH1750_CONTINOUS_L_RES_MODE)
        self.lux=(data >> 8 | (data&0xff)<<8)/1.2/2
        print(self.lux)