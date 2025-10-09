#include <Arduino.h>

#define RST 5           // pin 13 = port B pin 5
#define DAT 7           // pin 7 = port D pin 7
#define CLK 4           // pin 4 = port D pin 4 
#define BYTE_LENGTH 8


extern volatile uint8_t isr_timer2_counter;
extern volatile bool read_time_DS1302;

extern uint8_t last_measure;