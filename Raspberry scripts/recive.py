import paho.mqtt.client as mqtt
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
    global q
    q.put(msg)

client.on_connect = on_connect
client.on_message = on_message
q=Queue()
client.connect("192.168.0.23", 1883, 60)
client.subscribe("Control")
timeout = 1
i =''
duty = 0 # [%]
freq = 500 # [Hz]

# Pin Definitons :
pwmPin = 12 #< LED: Physical pin 12, BCM GPIO18

GPIO.setmode(GPIO.BOARD)
GPIO.setup(pwmPin , GPIO.OUT)

pwm = GPIO.PWM(pwmPin,freq)
pwm.start(duty)
#client.loop_start()  #Start loop 
try:
    while True:
        client.loop()
        # Wait for connection setup to complete
        while not q.empty():
            message = q.get()
            if message is None:
                continue
            print("received from queue",str(message.payload.decode("utf-8")))
            duty=float(message.payload.decode("utf-8"))
            duty=max(0,min(duty, 100))
            pwm.ChangeDutyCycle(float(message.payload.decode("utf-8")))
except KeyboardInterrupt:
    print('Koniec')
pwm.stop()
GPIO.cleanup()
client.disconnect()