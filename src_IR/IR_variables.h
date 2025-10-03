#include <Arduino.h>

#define BUTTON_0 22
#define BUTTON_1 12
#define BUTTON_2 24
#define BUTTON_3 94
#define BUTTON_4 8
#define BUTTON_5 28


extern volatile uint16_t IR_signal_width;
extern volatile uint16_t last_timer1;

extern volatile uint32_t received_bits;
extern volatile uint32_t received_value;