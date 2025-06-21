#include <Arduino.h>
#include "pins.h"


///// VARIABLES /////
uint8_t Columns = 15; // count from 0 --> 0-15 (16)
uint8_t Layers = 3; // count from 0 --> 0-3 (4)


///// TASKS /////
bool E_flags[4] = {false, false, false, false};
