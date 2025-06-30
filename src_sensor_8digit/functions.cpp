#include <Arduino.h>
#include "functions.h"
#include "dht11.h"

dht11 my_dht11;

bool start_sequence_1_flag = false;
bool start_sequence_2_flag = false;
bool last_state = false;
bool rising_edge_flag = false;
bool falling_edge_flag = false;
bool rising_edge_start_time = false;
bool falling_edge_start_time = false;

byte cycles = 0;
uint16_t offset = 0;
uint16_t start_time = 0;
uint16_t stop_time = 0;
uint8_t byte_num = 0;

void dht11_setup(void){

DDRD  |=  (1 << DDD7); // digital 7 as output 
PORTD |=  (1 << DDD7); // starts as HIGH

for(long i=0; i < 16000000; i++){  
// wait 1 sec to stabilize the sensor
}

}

int dht11::read(void){

// pin as output 
PORTD &=  ~(1 << DDD7); // LOW
start_sequence_1_flag = 1;

return 0;

}

