from timer import Reapeted_Timer
import time
from smbus2 import SMBus
from BMP280 import BMP280
import json
from PID import PID
try :
    import RPi.GPIO as GPIO
except RuntimeError :
    print (" Error importing RPi. GPIO ! Use 'sudo ' to run your script ")

def timer_callback():
    global BMP280_inst,duty,i,step_freq,f,pwm ,Setpoint,pid
    BMP280_inst.ReadTemp()
    duty=pid.Call(Setpoint,BMP280_inst.temp)
    pwm.ChangeDutyCycle(duty)
    msg={ "time":time.time(),"temp":BMP280_inst.temp,"u":duty}
    json_string=json.dumps(msg)
    print(json_string)
    f.write(json_string)
    f.write("\n")
duty = 0 # [%]
freq = 500 # [Hz]
i=0
# Pin Definitons :
pwmPin = 12 #< LED: Physical pin 12, BCM GPIO18
open('pomiary.txt', 'w').close()
f=open("pomiary.txt","a")
GPIO.setmode(GPIO.BOARD)
GPIO.setup(pwmPin , GPIO.OUT)

pwm = GPIO.PWM(pwmPin,freq)
pwm.start(duty)
BMP280_inst=BMP280(SMBus(1))
Setpoint=23.5
pid=PID(371.248,1.95,0,0.5,1.015,0.1)
TIM=Reapeted_Timer(Tp = 0.5,callback=timer_callback)
input("Press Enter to continue...")
TIM.done=True
# Disconnect from the MQTT broker
f.close()