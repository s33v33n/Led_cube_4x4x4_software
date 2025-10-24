#include <Arduino.h>

#define BYTE_LENGTH 8


// VARIABLES for LED_CUBE
extern uint8_t Columns; // count from 0 --> 0-15 (16)
extern uint8_t Layers;// count from 0 --> 0-3 (4)



// IR_variables 
#define BUTTON_0 22
#define BUTTON_1 12
#define BUTTON_2 24
#define BUTTON_3 94
#define BUTTON_4 8
#define BUTTON_5 28
#define BUTTON_6 90
#define BUTTON_7 66
#define BUTTON_8 82
#define BUTTON_9 74
#define BUTTON_PLAY 67
#define BUTTON_VOL_ADD 64
#define BUTTON_VOL_SUB 68
#define BUTTON_CH_SUB 69
#define BUTTON_CH_ADD 70
#define BUTTON_CH 71
#define BUTTON_SUB 7
#define BUTTON_ADD 21
#define BUTTON_EQ 9
#define BUTTON_FOL_SUB 25
#define BUTTON_FOL_ADD 13


extern volatile uint16_t IR_signal_width;
extern volatile uint16_t last_timer1;

extern volatile uint32_t received_bits;
extern volatile uint32_t received_value;




// RTC_variables
extern volatile uint8_t isr_timer2_counter;
extern volatile bool read_time_DS1302;