# Firmware overview
When looking for firmware don't forget to check the following projects: [sonoff/tasmota](https://github.com/arendst/Sonoff-Tasmota/tree/development/sonoff) and [espeasy](https://www.letscontrolit.com/wiki/index.php/ESPEasy). They are
universal firmwares that can do multiple things. Even though they're not difficult to use,
they all have their own mqtt topics, data formats und update procedures making them less intuitive.
Also their source code has grown over time, making it less easy to study it in a short time.

## Multiple, dedicated, firmwares
Instead of creating a great universal firmware that supports a multitude of shields this project consists of dedicated firmwares for a specific task.

If new shields are added it is easy to create the firmware because of the homie framework.
