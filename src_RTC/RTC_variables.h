#include <Arduino.h>

#define BYTE_LENGTH 8

#define RST 2           // pin 10 = port B pin 2
#define DAT 2           // pin 2 = port D pin 2
#define CLK 4           // pin 4 = port D pin 4 


extern volatile uint8_t isr_timer2_counter;
extern volatile bool read_time_DS1302;

