# Wemos D1 mini I2C diagnostic

This firmware is meant to be used on a Wemos D1. It's part of the [IoT Kit for education](https://www.iot-kit.nl) project.
It is based on the homie platform developed by Marvin Roger.
The original documentation can be found here: http://marvinroger.github.io/homie-esp8266/

This node serves as a sensor that reports distance via MQTT. The sensor used is a BH-1750.

# It's a diagnostic, nothing more

But proved to be very usefull when working with I2C shields.
It really helped me to identify the pins on the shield that we used for SDA and SCL.
