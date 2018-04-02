# Sensor DHT11

This firmware is meant to be used on a Wemos D1.
It has the ui_bundle embedded to allow for easy config.

It serves as a sensor that reports temperature and humidity via MQTT.

Upload /data contents first: pio run -t uploadfs
Then upload firmware: pio run -t upload

To change the config publish to topic: homie/<device-id>/$implementation/config/set

Incremental OTA changes for homie-ota.
{
  "name": "Sensor x",
  "device_id": "wemos-x",
  "mqtt": {
    "host": "192.168.xx.xx",
    "base_topic": "devices/",
    "auth": true,
    "username": "user",
    "password": "pass",
  },
  "ota": {
    "enabled": true,
    "host": "192.168.xx.xx",
    "port": 9080,
    "path": "/ota",
    "ssl": false
  }
}
