#include <Arduino.h>
#include <effects.h>
#include <variables.h>

// VARIABLES for LED_CUBE
uint8_t Columns = 15; // count from 0 --> 0-15 (16)
uint8_t Layers = 3; // count from 0 --> 0-3 (4)

// Effects 
Effect current_effect = Effect_0;

Task tasks[] = {

  {NO_TASK, nullptr},
  {TASK_0_INTERVAL, effect0_scope::effect0},
  {TASK_1_INTERVAL, effect1_scope::effect1},
  {TASK_2_INTERVAL, effect2_scope::effect2},
  {TASK_3_INTERVAL, effect3_scope::effect3},
  {TASK_4_INTERVAL, effect4_scope::effect4}
};

// IR_variables 
volatile uint16_t IR_signal_width = 0;
volatile uint16_t last_timer1 = 0;

volatile uint32_t received_bits = 0;
volatile uint32_t received_value = 0;


// RTC_variables 
volatile uint8_t isr_timer2_counter = 0;
volatile bool read_time_DS1302 = false;


// Task management 
uint32_t last_time = 0;