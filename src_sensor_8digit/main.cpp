/*
What need to setup for pin interrupt:
PCICR – Pin Change Interrupt Control Register: enable or disable interrupt for group of pins

PCMSKn – Pin Change Mask Register n: enable or disable change interrupt on the corresponding I/O pin

SREG – AVR Status Register: enable global interrupt

*/

#include <Arduino.h>
#include "functions.h"
#include "dht11.h"

volatile uint8_t value_A = 0;
volatile uint8_t value_B = 0;


void setup(){

    Serial.begin(115200);
    while (!Serial); // wait untill Serial is ready

    dht11_setup();
    Serial.println("After dht11 setup");
    timer1_setup();

    my_dht11.read();

    Serial.print(start_sequence_1_flag);
    Serial.print(" = start_sequence_1_flag, ");
    Serial.print(TOV1);
    Serial.println(" = TOV1");
}

ISR(PCINT2_vect){

    Serial.println(" I'm in ISR(PCINT2_vect)");

    if(only_one_first_rising_edge_flag){
        
        if(PIND & (1 << PIND7)){ // rising edge -> need to calculate low state time     
            rising_edge_start_time = TCNT1;
            first_rising_edge_interrupt_flag = true;

            Serial.print(rising_edge_start_time);
            Serial.println(" = rising_edge_start_time");
        }
    }

    if(PIND & ~(1 << PIND7)){ // falling egde 
        falling_edge_start_time = TCNT1;
        falling_edge_flag = true;

        Serial.print(falling_edge_start_time);
        Serial.println(" = falling_edge_start_time");
    }
}

ISR(TIMER1_COMPA_vect){ // ISR period is 16 ms  

value_A++;
  if (value_A == 10){
    value_A = 0;
    OCR0A++;
    OCR2A++;    
  }
}

ISR(TIMER1_COMPB_vect){

value_B++;
  if (value_B == 10){
    value_B = 0;
    OCR0B++;
    OCR2B++;
  }
}

void loop(){

    if (start_sequence_1_flag && !TOV1){
        cycles++;
        TIFR1 |= (1 << TOV1);

        Serial.print(cycles);
        Serial.print(" = cycles, ");
        Serial.print(TOV1);
        Serial.println(" = TOV1");


        if(cycles == 5){ 
            start_time = TCNT1; 
            if (65535 - start_time > 480){
                offset = 0;
            }
            else{
                offset = 65535 - start_time;
            }

            Serial.print(start_time);
            Serial.print(" = start_time, ");
            Serial.print(offset);
            Serial.println(" = offset");


            PORTD |=  (1 << DDD7); // HIGH 
            start_sequence_1_flag = 0;
            start_sequence_2_flag = 1;
            cycles = 0;
        }
    }

    if (start_sequence_2_flag && (TCNT1 - (start_time + offset)) > 480){

        Serial.println("Starting 2nd sequence");

        start_sequence_2_flag = 0;

        // pin D7 as input 
        DDRD  &=  ~(1 << DDD7); // digital 7 as input 

        // enable interrupt 
        PCICR  |= (1 << PCIF2);     // enable interrupt for group where is pin D7 
        PCMSK2 |= (1 << PCINT23);   // enable interrupt for pin D7

        last_time_stamp = TCNT1; // timestamp for falling edge -> time when pin was changed to input mode 

        Serial.print(last_time_stamp);
        Serial.println(" = last_time_stamp");

        only_one_first_rising_edge_flag = true; 
    }


    if(only_one_first_rising_edge_flag && first_rising_edge_interrupt_flag){ // rising edge - first rising egde 

        if (65535 - rising_edge_start_time > 1440){
            offset = 0;
        }
        else{
            offset = 65535 - rising_edge_start_time;
        }

        // first timestamp ~ 80 us test low signal from dht11
        // period is 62.5 ns. 80us / 62.5 ns = 1280. ; 1120 = 70us ; 1440 = 90 us ;  

        if(1120 < (start_time - rising_edge_start_time + offset) && (start_time - rising_edge_start_time + offset) < 1440){
            Serial.println("DHT11 OK - first LOW signal success");
        }
        else{
            Serial.println("DHT11 ERROR - first LOW signal fail");
        }

        last_time_stamp = rising_edge_start_time;

        only_one_first_rising_edge_flag = false;
        first_rising_edge_interrupt_flag = false;

        first_falling_edge_flag = true;
    }



    if(falling_edge_flag){ // falling edge 

        if (65535 - falling_edge_start_time > 1440){
            offset = 0;
        }
        else{
            offset = 65535 - falling_edge_start_time;
        }

        if(first_falling_edge_flag){ // first timestamp ~ 80 us test low signal from dht11
            // period is 62.5 ns. 80us / 62.5 ns = 1280. ; 1120 = 70us ; 1440 = 90 us ;  

            if(1120 < (last_time_stamp - falling_edge_start_time + offset) && (last_time_stamp - falling_edge_start_time + offset) < 1440){
                Serial.println("DHT11 OK - first HIGH signal success");
            }
            else{
                Serial.println("DHT11 ERROR - first HIGH signal fail");
            }

            first_falling_edge_flag = false; 
        }
        else{
            byte_num++;

            if(byte_num > 40){
                //pin change
                PCMSK2 &= ~(1 << PCINT23);   // disable interrupt for pin D7
                PCICR  &= ~(1 << PCIF2);     // disable interrupt for pins group
                DDRD  |=  (1 << DDD7); // digital 7 as output 
                PORTD |=  (1 << DDD7); // starts as HIGH
            }
            else{

                if( (last_time_stamp - falling_edge_start_time + offset) < 1500){
                    Serial.println("This is 0");
                }
                else{
                    Serial.println("This is 1");
                }
                
            }

        }

        falling_edge_flag = false;
        last_time_stamp = falling_edge_start_time;

    }

    // Serial.print(start_sequence_1_flag);
    // Serial.print(" = start_sequence_1_flag, ");
    // Serial.print(TOV1);
    // Serial.println(" = TOV1");
 
}