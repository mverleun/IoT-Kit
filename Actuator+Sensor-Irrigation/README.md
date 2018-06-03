# Wemos D1 mini as plant irrigation system

This firmware is meant to be used on a Wemos D1 Mini with a DHT-11 shield. It's part of the [IoT Kit for education](https://www.iot-kit.nl) project.
There are several shields available, make sure you get one that has the DHT-11 connected to D4 and *not* the I2C version.
Aliexpress has [this one](https://www.aliexpress.com/item/Free-Shipping-DHT-Shield-For-Wemos-D1-Mini-DHT11-Single-Bus-Digital-Temperature-And-Humidity-Sensor/32792325206.html) for sale.


## Connecting the hardware
Connect the hardware as follows:

| DHT11 |  Wemos D1 mini |
| --- | ---|
| VCC |+3.3V |
| GND | GND |
| SIG | D4 |


| Moisture Sensor |  Wemos D1 mini |
| --- | ---|
| VCC |+3.3V |
| GND | GND |
| SIG | A0 |

## MQTT
If all is configured well you should receive measurements on the following topics:

```
<base_topic>/<device_id>/temperature/temperature
<base_topic>/<device_id>/temperature/json
<base_topic>/<device_id>/humidity/humidity
<base_topic>/<device_id>/humidity/json
<base_topic>/<device_id>/moisture/humidity
<base_topic>/<device_id>/moisture/json
<base_topic>/<device_id>/watering/watering
```

Controlling the pump is done via
```
<base_topic>/<device_id>/watering/watering/set
```
which takes a value ON of OFF
Temperature and humidity are reported as floating point values.
The JSON output contains the name of the sensor, the name of the metric reported and the value of the measurement.
