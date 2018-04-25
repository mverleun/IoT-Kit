# IoT for education

In education you want focus on topics and not be distracted by avoidable side effects.

When starting to work on IoT classes it became clear that breadboards are nice, but hard to
troubleshoot. Loose contacts, misplaced wires etc. all distract from the IoT experience.

So it became clear that a cheap solution was needed, but it had to be robust. ESP8266 based
devices can be cheap and robust.
The Sonoff devices show this.They can be flashed with alternative firmwares. The same applies
to the Wemos D1.

There are excellent firmwares out there, [sonoff/tasmota](https://github.com/arendst/Sonoff-Tasmota/tree/development/sonoff), [espeasy](https://www.letscontrolit.com/wiki/index.php/ESPEasy) are a few, but they are
universal firmwares that can do multiple things. Even though they're not difficult to use,
they all have their own mqtt topics, data formats und update procedure making them inconsistent.
Also their source code has grown over time, making it less easy to study it in a short time.

For consistency there is a great framework developed by Marvin Roger called [Homie](https://github.com/marvinroger/homie-esp8266/).
This framework is the base for dedicated firmwares with consistent use of topics and standardized ways to report data.

The other advantage of using this framework is that it's easy to re-flash the controllers
OTA using [homie-ota](https://github.com/jpmens/homie-ota) which is developed by JP Mens, check out .

When combining these two excellent projects it became easy to develop dedicated firmware
with easy to understand source code.

Below is a list of firmwares already developed or in progress.


## Firmwares
The firmwares below are all firmwares with a specific purpose. Interchangabilty is the key
when using these firmwares.

Schools can have a limited number of Wemos D1's and add shields to it for a specific purpose.
Based on the purpose they can then flash these devices.

With exception of the Sonoff firmware all the other firmwares come with the UI for the initial configuration loaded on the device. This allows for easier setup.

### Actuator TB16612
A simple [motor driver shield](https://nl.aliexpress.com/item/Motor-Shield-For-WeMos-D1-mini-I2C-Dual-Motor-Driver-TB6612FNG-1A-V1-0-0/32700182142.html) for the Wemos D1. Currently not working due to issues with the TB16612

### Actuator-Sonoff
Firmware for basic [Sonoff S20](https://www.itead.cc/smart-socket.html) and [Sonoff Basic](https://www.itead.cc/smart-home/sonoff-wifi-wireless-switch.html) switches. Both the on/off button and MQTT commands can be used to turn the device on or off.

### Sensor-BH1750
A sensor that reports the intensity of the light in lux. It also has a topic that indicates
wether dusk has come or not. Meant to be run on the Wemos D1 with a shield based on the BH1750

### Sensor-DHT11
The infamous [DHT11 shield](https://nl.aliexpress.com/item/Free-Shipping-DHT-Shield-For-Wemos-D1-Mini-DHT11-Single-Bus-Digital-Temperature-And-Humidity-Sensor/32792325206.html) in combination with a Wemos D1. It reports temperature and humidity and is
to be used with a shield as well. Pay attention not to purchase the I2C version!



### Sensor-HC-SR04
Again for the Wemos D1. This time with a ultrasonic distance measuring shield.
Currently work in progress and no factory shield is available yet. You'll have to make it
yourselves.

### Sensor-OLED-Shield
The Wemos D1 again with a [OLED shield](https://nl.aliexpress.com/item/0-66-Inch-OLED-Shield-For-WeMos-D1-Mini-64X48-IIC-I2C-Compatible/32819429227.html). Also work in progress.


## Easy configuration
There are a couple of ways to configure the devices after flashing the firmware.

1. Using the built in configurator
2. Preloading the configuration by uploading a configuration file in JSON format.

If you deploy the second method is recommended. In each firmware directory there is a directory named data/homie. Inside is a file named config.json.example. Rename this file to config.json and change the settings to match your environment.
If you don't require authentication you can delete the following lines:

```
"username": "<mqtt username>",
"password": "<mqtt password>",
"auth":true
```

Make sure to remove the ',' behind "base_topic" or you end up with an invalid JSON file.

Everything inside the 'data' directory can be uploaded into the SPIFFS filesystem of the ESP8266 controller. In order to do so open a terminal in platformio and type the command:
`pio run -t uploadfs`. This will build the filesystem and upload it if your device is connected.
Next build the firmware and upload it the usual way to get a 'homie' firmware on your device.

Once the device contains 'homie' firmware you can reflash the device OTA. Upload the next build of your firmware to the 'homie-ota' server, select the device that you want to flash and proceed.

Check the documentation for the mentioned projects for details.
