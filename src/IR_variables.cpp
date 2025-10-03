#include <Arduino.h>

volatile uint16_t IR_signal_width = 0;
volatile uint16_t last_timer1 = 0;

volatile uint32_t received_bits = 0;
volatile uint32_t received_value = 0;