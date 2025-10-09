#include <Arduino.h>


volatile uint8_t isr_timer2_counter = 0;
volatile bool read_time_DS1302 = false;

uint8_t last_measure = 0;