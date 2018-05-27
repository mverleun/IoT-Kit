# Wemos D1 mini I2C diagnostic

This firmware is meant to be used on a Wemos D1. It's part of the [IoT Kit for education](https://www.iot-kit.nl) project.
It is based on the homie platform developed by Marvin Roger.
The original documentation can be found here: http://marvinroger.github.io/homie-esp8266/

# It's a diagnostic, nothing more

But proved to be very usefull when working with I2C shields.
It really helped me to identify the pins on the shield that we used for SDA and SCL. It also shows the address of the attached I2C shield.

It's the work from https://github.com/jainrk/i2c_port_address_scanner.git ported to the homie platform.
