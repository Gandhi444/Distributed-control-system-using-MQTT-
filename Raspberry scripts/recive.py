import paho.mqtt.client as mqtt
import time
import sys
import select
import numpy
from queue import Queue
try :
    import RPi.GPIO as GPIO
except RuntimeError :
    print (" Error importing RPi. GPIO ! Use 'sudo ' to run your script ")

client = mqtt.Client("efektor")

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print("re")
    q.put(msg)

client.on_connect = on_connect
client.on_message = on_message
q=Queue()
client.connect("192.168.1.33", 1883, 60)
client.subscribe("test")
timeout = 1
i =''
duty = 0 # [%]
freq = 500 # [Hz]

# Pin Definitons :
# pwmPin = 12 #< LED: Physical pin 12, BCM GPIO18

# GPIO.setmode(GPIO.BOARD)
# GPIO.setup(pwmPin , GPIO.OUT)

# pwm = GPIO.PWM(pwmPin,freq)
# pwm.start(duty)
client.loop_start()  #Start loop 
time.sleep(4)
try:
    while True:
        # Wait for connection setup to complete
        while not q.empty():
            message = q.get()
            if message is None:
                continue
            print("received from queue",str(message.payload.decode("utf-8")))
            #pwm.ChangeDutyCycle(str(message.payload.decode("utf-8")))
except KeyboardInterrupt:
    print('Koniec')
pwm.stop()
GPIO.cleanup()