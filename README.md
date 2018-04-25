# IoT for education

In education you want focus on topics and not be distracted by avoidable side effects.

When starting to work on IoT classes it became clear that breadboards are nice, but hard to 
troubleshoot. Loose contacts, misplaced wires etc. all distract from the IoT experience.

So it became clear that a cheap solution was needed, but it had to be robust. ESP8266 based 
devices can be cheap and robust.
The Sonoff devices show this.They can be flashed with alternative firmwares. The same applies 
to the Wemos D1.

There are excellent firmwares out there, sonoff/tasmota, espeasy are a few, but they are 
universal firmwares that can do multiple things. Even though they're not difficult to use, 
they all have their own mqtt topics, data formats und update procedure making them inconsistent.
Also their source code has grown over time, making it less easy to study it in a short time.

For consistancy there is a great framework developped by Marvin Roger called Homie.
This framework is the base for dedicated firmwares with consistent use of topics and standarized 
ways to report data.

The other advantage of using this framework is that it's easy to reflash the controllers 
OTA using Homie-OTA which is developped by JP Mensch.

When combining these two excellent projects it became easy to develop dedicated firmware
with easy to understand source code.

Below is a list of firmwares already developped or in progress.


## Firmwares
The firmwares below are all firmwares with a specific purpose. Interchangabilty is the key
when using these firmwares.

Schools can have a limitted number of Wemos D1's and add shields to it for a specific purpose.
Based on the purpose they can then flash these devices.

### Actuator TB16612
A simple motordriver for the Wemos D1. Currently not working due to issues with the TB16612

### Actuator-Sonoff
Firmware for basic Sonoff switches. Both the on/off button and MQTT commands can be used
to turn the device on or off.

### Sensor-BH1750
A sensor that reports the intensity of the light in lux. It also has a topic that indicates 
wether dusk has come or not. Meant to be run on the Wemos D1 with a shield based on the BH1750

### Sensor-DHT11
The infamous DHT11 in combination with a Wemos D1. It reports temperature and humidity and is 
to be used with a shield as well

### Sensor-HC-SR04
Again for the Wemos D1. This time with a ultrasonic distance measuring shield. 
Currently work in progress and no factory shield is available yet. You'll have to make it
yourselves.

### Sensor-OLED-Shield
The Wemos D1 again with a OLED shield. Also work in progress

