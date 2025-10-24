#include <Arduino.h>

/*
Digital:

PORT D (full):
(D0) PD0 -> TX
(D1) PD1 -> RX
(D2) PD2 -> RTC_module (DAT)
(D3) PD3 -> LED_CUBE (LAYER_1)
(D4) PD4 -> RTC_module (CLK)
(D5) PD5 -> LED_CUBE (LAYER_2)
(D6) PD6 -> LED_CUBE (LAYER_3)
(D7) PD7 -> Shift_register (CLK_CUBE)

PORTB:
(D8)  PB0 -> IR_receiver (IR_RECEIVER)
(D9)  PB1 -> Shift_register (SER_CUBE)
(D10) PB2 -> RTC_module (RST)
(D11) PB3 -> LED_CUBE (LAYER_4)
(D12) PB4 -> Shift_register (RESET_CUBE)
(D13) PB5 -> Shift_register (LATCH_CUBE)
*/


/* Numbers are reffering to the bit numbers in register, not to arduino pin numbers */

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
#define SER_CUBE 1      // Digital pin 9 = Port B bit 1 
#define RESET_CUBE 4    // Digital pin 12 = Port B bit 4  activates by low state 
#define LATCH_CUBE 5    // Digital pin 13 = Port B bit 5


// II) IR sensor 

///// REGISTER B /////
#define IR_RECEIVER 0    // Digital pin 8 = Port B bit 0


// III) RTC module (DS1302)

///// REGISTER D /////
#define DAT 2           // pin 2 = port D bit 2
#define CLK 4           // pin 4 = port D bit 4 

///// REGISTER B /////
#define RST 2           // pin 10 = port B bit 2




