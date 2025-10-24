#include <Arduino.h>

// VARIABLES for LED_CUBE
uint8_t Columns = 15; // count from 0 --> 0-15 (16)
uint8_t Layers = 3; // count from 0 --> 0-3 (4)


// IR_variables 
volatile uint16_t IR_signal_width = 0;
volatile uint16_t last_timer1 = 0;

volatile uint32_t received_bits = 0;
volatile uint32_t received_value = 0;


// RTC_variables 
volatile uint8_t isr_timer2_counter = 0;
volatile bool read_time_DS1302 = false;