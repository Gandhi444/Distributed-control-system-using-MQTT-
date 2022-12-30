#!/usr/bin/python3
import paho.mqtt.client as mqtt
import timer
import time

# Create an MQTT client instance
client = mqtt.Client("sensors")

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
    print(time.time())
    publish_message("test",time.time())
# Set the callbacks for connecting, receiving messages, and disconnecting
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
client.connect("192.168.1.33", 1883, 60)
# Publish a message to the "my/topic" topic
TIM=timer.Reapeted_Timer(increment = 0.1,callback=timer_callback)
# input("Press Enter to continue...")
# TIM.done=True
# # Disconnect from the MQTT broker
# client.disconnect()