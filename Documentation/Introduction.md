# IoT for education

In education you want focus on learning objectivs and not be distracted by avoidable side effects.
It's about the **big picture** where a lot of skills are required. 

Teaching IoT is more than doing some programming in Python or C(++) to read sensor values or turn a LED on.
It's about sensors, microcontrollers, information storage, etc. It's also about connectivity, bluetooth, wifi, LoraWan and more.

When working with novice students who who are not fully skilled it's important to make IoT easy accessible.

So no breadboards with loose or misplaced wires and sensors, but a robust collection of controllers, with shield, that are affordable hardware and interchangable.

An aspect often overlooked is the ability to update IoT devices remotely, sometimes in great quantities.

The ESP8266 is a great controller for these kind of devices. It's cheap, has WiFi connectivity and there are many shields available with sensors.

The [homie convention](https://github.com/homieiot/convention) establishes a standard for IoT devices and is rapidly adopted as such. It also allows to update many devices at once for ESP8266 based devices. (If there is enough storage available).

The homie convention is available for ESP8266 based devices, but also as a Python framework. It avoids a lot of coding which results in clean firmwares that are easy to understand, even for novice students whilest meeting the earlier mentioned requirments.
