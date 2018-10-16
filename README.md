# Cryo Heater Control
### MSP430FR4133-based temperature control for cryogenic system.

This code allows an MSP430FR4133 to fetch temperature from an SI diode via ADC and lookup table.

Data is displayed on LCD and can also be delivered over UART connection.

Currently, temperature data is sent in response to an incoming carriage return.

Additional functionality would require more complex messages, current idea is a simple version of SCPI protocol.

##### TODO:

 * SCPI-Lite Protocol (or other)
 * Heater Control
  * PWM
  * Output power ADC
  * Output power display