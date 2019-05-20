#!/usr/bin/python

try:
    import RPi.GPIO as GPIO
    import time
except RuntimeError:
    print("Error importing RPi.GPIO!  This is probably because you need superuser privileges.  You can achieve this by using 'sudo' to run your script")

print("Running on a Raspberry Pi version ",GPIO.RPI_INFO['P1_REVISION'])

LED1=6
LED2=18
LED3=27
LED4=22
LED5=23
LED6=24
LED7=17
SWITCH1=5
SWITCH2=18
GPIO.setmode(GPIO.BCM)
GPIO.setwarnings(False)

GPIO.setup(LED1, GPIO.OUT)
GPIO.setup(LED2, GPIO.OUT)
GPIO.setup(LED3, GPIO.OUT)
GPIO.setup(LED4, GPIO.OUT)
GPIO.setup(LED5, GPIO.OUT)
GPIO.setup(LED6, GPIO.OUT)
GPIO.setup(LED7, GPIO.OUT)

print("Starting test 1: LED's")
print("Press Switch 1 to move to the next test")
      
LOOP=True
GPIO.setup(SWITCH1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
while LOOP:
    for led in LED1, LED2, LED3, LED4, LED5, LED6, LED7, LED6, LED5, LED4, LED3, LED2, LED1:
        GPIO.output(led, GPIO.HIGH)
        time.sleep(0.1)
        GPIO.output(led, GPIO.LOW)
        if ((GPIO.input(SWITCH1))):
            LOOP=False
    for led in LED1, LED2, LED3, LED4, LED5, LED6, LED7:
        GPIO.output(led, GPIO.HIGH)
        time.sleep(0.1)
        if ((GPIO.input(SWITCH1))):
            LOOP=False
    for led in LED1, LED2, LED3, LED4, LED5, LED6, LED7:
        GPIO.output(led, GPIO.LOW)
        time.sleep(0.1)
        if ((GPIO.input(SWITCH1))):
            LOOP=False
    for led in LED7, LED6, LED5, LED4, LED3, LED2, LED1:
        GPIO.output(led, GPIO.HIGH)
        time.sleep(0.1)
        if ((GPIO.input(SWITCH1))):
            LOOP=False
    for led in LED7, LED6, LED5, LED4, LED3, LED2, LED1:
        GPIO.output(led, GPIO.LOW)
        time.sleep(0.1)
        if ((GPIO.input(SWITCH1))):
            LOOP=False

print("Starting test 2")
print("Press on either switch. A LED should turn on and off.")
print("Press both switches at the same time to proceed.")
GPIO.setup(SWITCH1, GPIO.IN, pull_up_down=GPIO.PUD_UP)
GPIO.setup(SWITCH2, GPIO.IN, pull_up_down=GPIO.PUD_UP)

while not (GPIO.input(SWITCH1) and GPIO.input(SWITCH2)):
    if GPIO.input(SWITCH1):
        GPIO.output(LED1, GPIO.HIGH)
        print("Switch 1")
        time.sleep(0.5)
    else:
        GPIO.output(LED1, GPIO.LOW)
    if GPIO.input(SWITCH2):
        GPIO.output(LED7, GPIO.HIGH)
        print("Switch 2")
        time.sleep(0.5)
    else:
        GPIO.output(LED7, GPIO.LOW)
    
GPIO.cleanup()
print("Done with test")
