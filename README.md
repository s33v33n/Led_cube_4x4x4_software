# Led Cube - 3D display & BCD clock

The purpose of this project was to create an electronic decatation. 
Led cube is designed to be controlled via IR remote. It is possible to change visual effects or show the time in BCD format - 1 hour timer, by pressing corresponding buttons. 

Hardware: 
- 64 x blue LED
- custom led_cube
    - 4 layers, each connected to digital pin (VCC)
    - 16 columns, each contains 4 LEDs and connected to digital pin (GND)
- 2 x shift regiters (sn74hc595)
- IR sensor (VS1828B) + remote
- RTC (DS1302)
- Arduino Nano (ATmega328P)

Software:
- Controlling GPIO from registers
- ISR
- Timers
- Own functions instead of using external libraries for the hardware
