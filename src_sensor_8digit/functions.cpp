#include <Arduino.h>
#include "functions.h"
#include "dht11.h"

dht11 my_dht11;

bool start_sequence_1_flag = false;
bool start_sequence_2_flag = false;
bool last_state = false;
bool rising_edge_flag = false;
bool falling_edge_flag = false;
bool only_one_first_rising_edge_flag = false;
bool first_falling_edge_flag = false;
bool main_flag = true;
bool first_rising_edge_interrupt_flag = false;


byte cycles = 0;
uint16_t offset = 0;
uint16_t start_time = 0;
uint16_t stop_time = 0;
uint16_t rising_edge_start_time = 0;
uint16_t falling_edge_start_time = 0;
uint16_t last_time_stamp = 0;
uint8_t byte_num = 0;

void dht11_setup(void){

    DDRD  |=  (1 << DDD7); // digital 7 as output 
    PORTD |=  (1 << DDD7); // starts as HIGH

    // for(long i=0; i < 16000000; i++){  
    // // wait 1 sec to stabilize the sensor
    // }

    delay(1000);

    Serial.println("DHT11 initialized");
}

int dht11::read(void){

// pin as output 
PORTD &=  ~(1 << DDD7); // LOW
start_sequence_1_flag = 1;

Serial.println("Set the start_sequence_1_flag");

return 0;

}

void timer1_setup(void){

// 0. Reset registers 
TCCR1B = 0;
TCCR1A = 0;
TIMSK1 = 0;

// 1. Enable clock
TCCR1B |=  (1 << CS10);      //  1      001 - enable internal clock, prescaler = 1   
TCCR1B &= ~(1 << CS11);      //  0      16 MHz 
TCCR1B &= ~(1 << CS12);      //  0      Period is  1/16 MHz

// 2. Counting mode 
TCCR1A &= ~(1 << WGM10); // 0       0000 - Normal operation mode TOP - 0xFFFF , set TOV Flag on MAX (0xFFFF). OCR0A and OCR0B are disabled
TCCR1A &= ~(1 << WGM11); // 0
TCCR1B &= ~(1 << WGM12); // 0
TCCR1B &= ~(1 << WGM13); // 0

// 3. Enable interrupt on compare match 
TIMSK1 |= (1 << OCIE1B); 
TIMSK1 |= (1 << OCIE1A);

}

