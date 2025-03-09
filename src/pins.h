#include <Arduino.h>

/* Numbers are reffering to bit numbers in register, not to arduino pin numbers */

///// REGISTER B /////
#define layer1 1    // Pin 9  = Port B bit 1
#define LATCH  4    // Pin 12 = Port B bit 4
#define layer3 3    // Pin 11 = Port B bit 3
#define layer4 2    // Pin 10 = Port B bit 2 
#define RESET 0     // Pin 8  = Port B bit 0   activates by low state 

///// REGISRER D /////
#define SER 7       // Pin 7  = Port D bit 7
#define layer2 3    // Pin 3  = Port D bit 3
#define CLK 4       // Pin 4  = Port D bit 4
#define IR_input 2  // Pin 2  = Port D bit 2


///// VARIABLES /////
extern uint8_t Columns; // count from 0 --> 0-15 (16)
extern uint8_t Layers;// count from 0 --> 0-3 (4)