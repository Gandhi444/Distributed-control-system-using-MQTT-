#!/usr/bin/python3
import paho.mqtt.client as mqtt
from timer import Reapeted_Timer
import time
#from smbus2 import SMBus
#from BMP280 import BMP280
import json
# Create an MQTT client instance
client = mqtt.Client("test")

# The callback for when the client receives a CONNACK response from the server.
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    # Subscribing in on_connect() means that if we lose the connection and
    # reconnect then subscriptions will be renewed.

# The callback for when a PUBLISH message is received from the server.
def on_message(client, userdata, msg):
    print(msg.topic+" "+str(msg.payload))

def publish_message(topic, payload):
    client.publish(topic, payload)
def timer_callback():
    global BMP280_inst
   # BMP280_inst.ReadTemp()
    #print(BMP280_inst.temp)
    #msg={ "time":time.time(),"temp":BMP280_inst.temp}
    publish_message("Control",0.0)
# Set the callbacks for connecting, receiving messages, and disconnecting
client.on_connect = on_connect
client.on_message = on_message
# Connect to the MQTT broker
#BMP280_inst=BMP280(SMBus(1))
client.connect("192.168.0.23", 1883, 60)
# Publish a message to the "my/topic" topic
TIM=Reapeted_Timer(Tp = 0.1,callback=timer_callback)
input("Press Enter to continue...")
TIM.done=True
# Disconnect from the MQTT broker
client.disconnect()