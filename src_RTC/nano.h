

#ifndef NANO_H
#define NANO_H

#include <Arduino.h>

#define RST 3   // bit number in PORT C (analog)
#define SCLK 6  // bit number in PORT D (digital)
#define DATA 5  // bit number in PORT D (digital)

void nano_setup(byte rst, byte sclk, byte data);
void data_pin_as_input(byte pin);
void data_pin_as_output(byte pin);


#endif //  NANO_H