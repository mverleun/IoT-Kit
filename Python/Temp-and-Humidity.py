#!/usr/bin/python
# (c) 2017 - MarCoach - Marco Verleun
import RPi.GPIO as GPIO
import sys
import Adafruit_DHT
import time
import os

SWITCH1=5
DHT11=16

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)

GPIO.setup(SWITCH1, GPIO.IN, pull_up_down=GPIO.PUD_UP)

LOOP=True
while LOOP:
    if ((GPIO.input(SWITCH1))):
        LOOP=False

    humidity, temperature = Adafruit_DHT.read_retry(11, DHT11)
    
    if humidity is not None or temperature is not None:
        print ('Temp={0:0.1f}*C  Humidity={1:0.1f}%'.format(temperature, humidity))
    else:
        print('Failed to get reading. Try again!')
    time.sleep(3)
   
GPIO.cleanup()
print("Test finished")
