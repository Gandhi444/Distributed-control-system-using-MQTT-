from timer import Reapeted_Timer
import time
from smbus2 import SMBus
from BH1750 import BH1750
import json
try :
    import RPi.GPIO as GPIO
except RuntimeError :
    print (" Error importing RPi. GPIO ! Use 'sudo ' to run your script ")

def timer_callback():
    global BH1750_inst,duty,i,step_freq,f,pwm
    BH1750_inst.ReadLux()
   # duty=10*(int(i/step_freq)%11)
    #duty=max(0,min(duty, 100))
    if i<100:
        duty=0
    else:
        duty=100
    pwm.ChangeDutyCycle(duty)
    msg={ "time":time.time(),"lux":BH1750_inst.lux,"u":duty}
    json_string=json.dumps(msg)
    print(json_string)
    f.write(json_string)
    f.write("\n")
    i+=1
duty = 0 # [%]
freq = 500 # [Hz]
i=0
step_freq=20
# Pin Definitons :
pwmPin = 12 #< LED: Physical pin 12, BCM GPIO18
open('pomiary.txt', 'w').close()
f=open("pomiary.txt","a")
GPIO.setmode(GPIO.BOARD)
GPIO.setup(pwmPin , GPIO.OUT)

pwm = GPIO.PWM(pwmPin,freq)
pwm.start(duty)
BH1750_inst=BH1750(SMBus(1))
TIM=Reapeted_Timer(Tp = 0.015,callback=timer_callback)
input("Press Enter to continue...")
TIM.done=True
# Disconnect from the MQTT broker
f.close()