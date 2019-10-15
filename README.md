# CY8KIT-59 Cypress PSoC 5LP Nixie Clock Demo

This is my first ever project using a Cypress Microcontroller. It's the result of only about 4 hours of development from never having touched the device to getting the following demo. 

The devices does the following:
1. Reads ADC Pin to Adjust Brigntness
2. Writes 8-bit ADC Value as a PWM Duty Cycle to control Nixie Tube Brightness
3. Talks over SPI to Daisy-chained Microchip HV5530 High Voltage Drievers used for the Nixies (32 bit SIPO Shift Registers Essentially)
4. Talks over i2C to a Maxim DS3231 Real Time Clock to get the time
5. Outputs Time and Date over Serial UART at 115200 Baud (for debug and because why not)

I also implemented my own delay function using a timer and interrupt because I thought it would be good practice. 

A video of it running can be found here:
[https://www.youtube.com/watch?v=CJh9BvjUm9Y]https://www.youtube.com/watch?v=CJh9BvjUm9Y