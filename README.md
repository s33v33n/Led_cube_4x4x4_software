The purpose of this project was to build a prototype led_cube 4x4x4 and write software for it. Led_cube has 4 visual effects and could display time in BCD format.
Used hardware: 
- custom led_cube
- 2 x shift regiters (sn74hc595)
- IR sensor (VS1828B) + remote
- RTC (DS1302)
- Arduino Nano (Atmega328P)

Software:
- Controlling GPIO from registers
- ISR
- Timers
- Own functions instead of using external libraries for used hardware
