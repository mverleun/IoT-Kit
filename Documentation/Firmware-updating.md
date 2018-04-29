
# Updating/changing the firmware
Updating the firmware can be done using the homie-ota application. This is the easiest way and it allows to *change* the firmware to one with a different function.
homie-ota also allows for downgrading.

It is also possible to remotely update the device using command line tools.
Check [ota_updater.py](https://github.com/marvinroger/homie-esp8266/tree/develop/scripts/ota_updater) for installation instructions and how to use it.

An example update session looks like this:

```
$ ota_updater.py -l mqtt.example.com -p 1883 \
					-u <mqtt-user> -d <mqtt-password> \
					-t "devices/" -i wemos-X firmware.bin
Connecting to mqtt broker mqtt.example.com on port 1883
Connected with result code 0
Waiting for device info...
Publishing new firmware with checksum e6197d4febf2a24a5a14b3ca71636679
[++++++++++++++++++++++++++++++] 409024/409024
Device back online. Update Successful!
```

