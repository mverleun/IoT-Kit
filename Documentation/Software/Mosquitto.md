# Mosquitto  
[Mosquitto](https://mosquitto.org) is a MQTT server and the connecting element between the devices and the management server.

## homie topics
The devices subscribe to topics on this server as specified in the homie convention.
The also periodically publish information to topics.

The homie topics used to publish and subscribe form the base of functioning of homie-ota.

## Firmware topics
Besides the default topics, homie also allows defining additional topics. These are used to publish sensor readings, but also allow for remote instructions if the device is an actuator.

## Using wildcards in topics
MQTT allows the usage of the wildcards `+` and `#`.

The `+` is a wildcard for a specific level in the topic name, the `#` is used to get all topics and subtopics from the level where the `#` is used.

To check the status of the devices one can subscribe to `devices/+/$online`. To get all the temperature readings from devices subscribe to `devices/+/temperature/temperature`

To subscribe to all topics and subtopics of a device use `devices/<device-id>/#`

## mosquitto configuration
Below is an example of the configuration that can be used with the IoT Kit devices. 
This configuration is used with mosquitto running in a docker container.
If you're not running from a docker container you might want to change the config.

```
listener 1883
pid_file /var/run/mosquitto.pid
persistence true
persistence_location /mosquitto/data/db/
allow_zero_length_clientid false
persistent_client_expiration 5d
log_dest file /mosquitto/log/mosquitto.log
log_dest topic
allow_anonymous true

listener 8883
cafile /mosquitto/config/certs/CA.crt
certfile /mosquitto/config/certs/mqtt.example.com.crt
keyfile /mosquitto/config/certs/mqtt.example.com.key
allow_zero_length_clientid false

listener 9001
protocol websockets
cafile /mosquitto/config/certs/CA.crt
certfile /mosquitto/config/certs/mqtt.example.com.crt
keyfile /mosquitto/config/certs/mqtt.example.com.key

# Logging settings
connection_messages true
log_timestamp true
log_type error
log_type warning
log_type information
log_type notice
log_type subscribe
log_type unsubscribe
```

## Monitoring MQTT messages
There are several great apps that you can use to 'spy' on MQTT messages being published. One worth mentioning is [mqtt-spy](https://github.com/eclipse/paho.mqtt-spy).
![MQTT Spy](https://raw.githubusercontent.com/mverleun/IoT-devices/master/Documentation/Software/Images/mqtt-spy.png)

## Troubleshooting MQTT
If you experience connection issues, random disconnects, check to make sure that each device has a unique name. The name is the only attribute that MQTT has to identify a device. 
The name used to identify the device with MQTT is a concatination of the words `Homie-` and the device-id, e.g. `Homie-wemos-4`

In the logfile of mosquitto can check if a device is connected and to which topics the device has subscribed if you have the proper settings in the mosquitto config file.

```
1524913539: New client connected from 8x.xxx.xxx.xx2 as Homie-wemos-4 (c1, k15, u'xxx').
1524913539: Homie-wemos-4 1 devices/wemos-4/$implementation/ota/firmware/+
1524913539: Homie-wemos-4 1 devices/wemos-4/$implementation/reset
1524913539: Homie-wemos-4 1 devices/wemos-4/$implementation/config/set
1524913539: Homie-wemos-4 2 devices/wemos-4/+/+/set
1524913539: Homie-wemos-4 2 devices/$broadcast/+
```

Another common mistake is with the name of topics and subtopics. The IoT Kit topics start with `device/`, that is without a `/` at the front.

