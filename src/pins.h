#include <Arduino.h>

/* Numbers are reffering to bit numbers in register, not to arduino pin numbers */

// I) LED_CUBE 

// a) Layers

///// REGISTER D /////
#define LAYER_1 3    // Digital pin 3 = Port D bit 3
#define LAYER_2 5    // Digital pin 5 = Port D bit 5
#define LAYER_3 6    // Digital pin 6 = Port D bit 6 

///// REGISTER B /////
#define LAYER_4 3    // Digital pin 11 = Port B bit 3 


// b) Shift register

///// REGISTER D /////
#define CLK_CUBE 7       // Digital pin 7 = Port D bit 7

///// REGISTER B /////
#define SER_CUBE 0      // Digital pin 8 = Port B bit 0 
#define RESET_CUBE 4    // Digital pin 12 = Port B bit 4  activates by low state 
#define LATCH_CUBE 5    // Digital pin 13 = Port B bit 5



// 


///// VARIABLES /////
extern uint8_t Columns; // count from 0 --> 0-15 (16)
extern uint8_t Layers;// count from 0 --> 0-3 (4)


///// TASKS /////
extern bool E_flags[];
 