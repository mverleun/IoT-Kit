# Software components

## Homie
[Homie](https://github.com/marvinroger/homie-esp8266/) is developped by Marvin Roger. It provides a great framework for ESP8266 based IoT devices. 

The concept of homie has been extended to other environments as well. This allows you to create python applications that conform to the homie standard, creating a homegenous environment.

Using homie allows us to keep the source code of our sensors and actuators focussed on acomplishing their tasks and not waste time on writing code to interact with the MQTT server directly.

Configuration management is also handled by this framework and it allows remote updates OTA (Over The Air)(ESP8266 Only).

This is great if you rapidly want to change firmware on a controller to change it task, e.g. when you attach a different shield.



## Homie OTA
[homie-ota](https://github.com/jpmens/homie-ota) which is developed by JP Mens is used as a management solution. It is easy to use and has a lot of functionallity. We use it to change firmware on the devices remotely, but it also shows sensor readings and the status of the device. It is invaluable in the total setup.

## Mosquitto  
[Mosquitto](https://mosquitto.org) is the MQTT server and the connecting element between the devices and the management server.

## Nodered
[Nodered](https://nodered.org) is used to develop flows. It requires some configuration and it allows for programming, but the majority of solutions can be done without programming.

## Grafana


## Influxdb

## Logstash