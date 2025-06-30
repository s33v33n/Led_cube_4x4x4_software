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
extern bool rising_edge_start_time;
extern bool falling_edge_start_time;

extern byte cycles;
extern uint16_t offset;
extern uint16_t start_time;
extern uint16_t stop_time;
extern uint8_t byte_num;

void dht11_setup(void);

#endif