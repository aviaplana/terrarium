# Terrarium

Controller for a terrarium. It operates:
- Lights
- Internal air flow fan
- Input air fan
- Ultrasonic humidifier

Mantains the humidity of the terrarium between 95 and 99 %

A bme280 sensor is used to read humidity and temperature.

Sends the status periodically to the mqtt server.

It also stores the light status and timer to the EEPROM. If the controller resets, it will recover the previous status.