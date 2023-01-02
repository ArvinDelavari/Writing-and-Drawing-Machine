# Writing Machine
## IUST - April 2022
- main.py includes python code which get .svg file as input and and changes it to linear movement in X and Y axis. The converted data is sent to MQTT server and received by ESP8266 MCU.
- Node-Serial is .ino code for ESP8266 MCU. It receives data from MQTT server. Data will be converted to a string and sent by serial(UART) to main MCU.

## Arvin Delavari - Mohammad Arman Yazdi
## Iran University of Science and Technology
