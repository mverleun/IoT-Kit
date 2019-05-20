#!/usr/bin/python
import smbus
import time
import RPi.GPIO as GPIO
    
# Define some constants from the datasheet
DEVICE     = 0x23 # Default device I2C address
POWER_DOWN = 0x00 # No active state
POWER_ON   = 0x01 # Power on
RESET      = 0x07 # Reset data register value
 
# Start measurement at 4lx resolution. Time typically 16ms.
CONTINUOUS_LOW_RES_MODE = 0x13
# Start measurement at 1lx resolution. Time typically 120ms
CONTINUOUS_HIGH_RES_MODE_1 = 0x10
# Start measurement at 0.5lx resolution. Time typically 120ms
CONTINUOUS_HIGH_RES_MODE_2 = 0x11
# Start measurement at 1lx resolution. Time typically 120ms
# Device is automatically set to Power Down after measurement.
ONE_TIME_HIGH_RES_MODE_1 = 0x20
# Start measurement at 0.5lx resolution. Time typically 120ms
# Device is automatically set to Power Down after measurement.
ONE_TIME_HIGH_RES_MODE_2 = 0x21
# Start measurement at 1lx resolution. Time typically 120ms
# Device is automatically set to Power Down after measurement.
ONE_TIME_LOW_RES_MODE = 0x23
 
#bus = smbus.SMBus(0) # Rev 1 Pi uses 0
bus = smbus.SMBus(1)  # Rev 2 Pi uses 1

def convertToNumber(data):
  # Simple function to convert 2 bytes of data
  # into a decimal number
  return ((data[1] + (256 * data[0])) / 1.2)
 
def readLight(addr=DEVICE):
  data = bus.read_i2c_block_data(addr,ONE_TIME_HIGH_RES_MODE_1)
  return convertToNumber(data)




    
LED1=6
LED2=18
LED3=27
LED4=22
LED5=23
LED6=24
LED7=17
SWITCH1=5
SWITCH2=18

GPIO.setwarnings(False)

GPIO.setmode(GPIO.BCM)

GPIO.setup(LED1, GPIO.OUT)
GPIO.setup(LED2, GPIO.OUT)
GPIO.setup(LED3, GPIO.OUT)
GPIO.setup(LED4, GPIO.OUT)
GPIO.setup(LED5, GPIO.OUT)
GPIO.setup(LED6, GPIO.OUT)
GPIO.setup(LED7, GPIO.OUT)

GPIO.setup(SWITCH1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
def main():
  print("Press switch 1 to exit.")
  print("Change the intensity of the light on the sensor")
  print("and observe the LED's")

  LOOP=True
  while LOOP:
    if ((GPIO.input(SWITCH1))):
            LOOP=False
    reading=readLight()
    for led in LED1, LED2, LED3, LED4, LED5, LED6, LED7:
        GPIO.output(led, GPIO.LOW)
    time.sleep(0.01)
    GPIO.output(LED1, GPIO.HIGH)
    if reading > 10:
        GPIO.output(LED2, GPIO.HIGH)
    if reading > 100:
        GPIO.output(LED3, GPIO.HIGH)
    if reading > 200:
        GPIO.output(LED4, GPIO.HIGH)
    if reading > 300:
        GPIO.output(LED5, GPIO.HIGH)
    if reading > 500:
        GPIO.output(LED6, GPIO.HIGH)
    if reading > 750:
        GPIO.output(LED7, GPIO.HIGH)
    time.sleep(1)
    
  GPIO.cleanup()
  print("Test finished")


if __name__=="__main__":
   main()
