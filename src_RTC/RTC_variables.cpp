#include <Arduino.h>

volatile uint8_t isr_timer2_counter = 0;
volatile bool read_time_DS1302 = false;

uint8_t last_measure = 0;





enum read_commands{

    READ_SECONDS = 0x81,
    READ_MINUTES = 0x83,
    READ_HOURS = 0x85
};
