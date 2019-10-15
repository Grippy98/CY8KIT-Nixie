# CY8KIT-59 Cypress PSoC 5LP IN-12 Nixie Clock Demo

This is my first ever project using a Cypress Microcontroller. It's the result of only about 4 hours of development from never having touched the device to getting the following demo. 

It's a clock driver for Old Soviet IN-12 Nixie Tubes.  

![alt text](https://github.com/Grippy98/CY8KIT-Nixie/blob/master/images/clock.jpg "The clock")

### A video of it running can be found here:
https://www.youtube.com/watch?v=CJh9BvjUm9Y

### The device does the following:
1. Reads ADC Pin to Adjust Brigntness
2. Writes 8-bit ADC Value as a PWM Duty Cycle to control Nixie Tube Brightness
3. Talks over SPI to Daisy-chained Microchip HV5530 High Voltage 32 bit SIPO Drivers used for the IN-12 Nixies
4. Talks over i2C to a Maxim DS3231 Real Time Clock to get the time
5. Outputs Time and Date over Serial UART at 115200 Baud (for debug and because why not)

I also implemented my own delay function using a timer and interrupt because I thought it would be good practice. 

## Top Level Design:
![alt text](https://github.com/Grippy98/CY8KIT-Nixie/blob/master/images/Top_Design.PNG "Top Level Design")


## Pin Mappings:
![alt text](https://github.com/Grippy98/CY8KIT-Nixie/blob/master/images/Pin_assignments.PNG "Pin Mappings")