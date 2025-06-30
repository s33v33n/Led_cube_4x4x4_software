/*
What need to setup for pin interrupt:
PCICR – Pin Change Interrupt Control Register: enable or disable interrupt for group of pins

PCMSKn – Pin Change Mask Register n: enable or disable change interrupt on the corresponding I/O pin

SREG – AVR Status Register: enable global interrupt

*/

#include <Arduino.h>
#include "functions.h"
#include "dht11.h"


void setup(){

    Serial.begin(115200);
    while (!Serial); // wait untill Serial is ready

    dht11_setup();
}

ISR(PCINT2_vect){

if(DDD7 && !last_state){ // rising edge 
    
    rising_edge_start_time = TCNT1;
    rising_edge_flag = true;
    falling_edge_flag = false;
    last_state = true;
}

if(!DDD7 && last_state){ // falling egde 

    falling_edge_start_time = TCNT1;
    rising_edge_flag = false;
    falling_edge_flag = true;
    last_state = false;
}

}

void loop(){
    
    if (start_sequence_1_flag && !TOV1){
        cycles++;
        TIFR1 |= (1 << TOV1);
        if(cycles == 5){ 
            start_time = TCNT1; 
            if (65535 - start_time > 480){
                offset = 0;
            }
            else{
                offset = 65535 - start_time;
            }
            PORTD |=  (1 << DDD7); // HIGH 
            start_sequence_2_flag = 1;
            start_sequence_1_flag = 0;
            cycles = 0;
        }
    }

    if (start_sequence_2_flag && (TCNT1 - (start_time + offset)) > 480){
        
        start_sequence_2_flag = 0;
        //PORTD &=  ~(1 << DDD7); // pin LOW
        last_state = true;

        // pin as input 
        DDRD  &=  ~(1 << DDD7); // digital 7 as input 

        // enable interrupt 
        PCICR  |= (1 << PCIF2);     // enable interrupt for group where is pin D7 
        PCMSK2 |= (1 << PCINT23);   // enable interrupt for pin D7

        falling_edge_start_time = TCNT1; // timestamp for falling edge 

        rising_edge_flag = true; // need to change this flag, because of only one timestamp -> wrong calculation (need 2 timestamps = x_t1 - x_t1) 

        /* First ISR change is on rising egde (after 80 us LOW test signal)
        
        
        */

    }

    if(rising_edge_flag && !falling_edge_flag){ // rising edge 
        cycles++;
        stop_time = TCNT1;

        if (65535 - rising_edge_start_time > 1440){
            offset = 0;
        }
        else{
            offset = 65535 - rising_edge_start_time;
        }

        if(cycles == 1){ // first timestamp ~ 80 us test low signal from dht11
            // period is 62.5 ns. 80us / 62.5 ns = 1280. ; 1120 = 70us ; 1440 = 90 us ;  

            if(1120 < (stop_time - rising_edge_start_time + offset) && (stop_time - rising_edge_start_time + offset) < 1440){

            }
            else{
                Serial.println("DHT11 ERROR - first low signal fail");
            }
        }

        else{
            byte_num++;

        }

    }

    if(falling_edge_flag && !rising_edge_flag){ // falling edge 

        cycles++;
        stop_time = TCNT1;

        if (65535 - falling_edge_start_time > 1440){
            offset = 0;
        }
        else{
            offset = 65535 - falling_edge_start_time;
        }

        if(cycles == 2){ // first timestamp ~ 80 us test low signal from dht11
            // period is 62.5 ns. 80us / 62.5 ns = 1280. ; 1120 = 70us ; 1440 = 90 us ;  

            if(1120 < (stop_time - falling_edge_start_time + offset) && (stop_time - falling_edge_start_time + offset) < 1440){

            }

            else{
                Serial.println("DHT11 ERROR - first high signal fail");
            }
        }
    
    }

}