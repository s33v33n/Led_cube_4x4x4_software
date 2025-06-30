#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include <Arduino.h>
#include "dht11.h"

extern dht11 my_dht11;

extern bool start_sequence_1_flag;
extern bool start_sequence_2_flag;
extern bool last_state;
extern bool rising_edge_flag;
extern bool falling_edge_flag;
extern bool only_one_first_rising_edge_flag;
extern bool first_falling_edge_flag;
extern bool main_flag;
extern bool first_rising_edge_interrupt_flag; 

extern byte cycles;
extern uint16_t offset;
extern uint16_t start_time;
extern uint16_t stop_time;
extern uint16_t last_time_stamp;
extern uint16_t rising_edge_start_time;
extern uint16_t falling_edge_start_time;
extern uint8_t byte_num;

void dht11_setup(void);
void timer1_setup(void);

#endif