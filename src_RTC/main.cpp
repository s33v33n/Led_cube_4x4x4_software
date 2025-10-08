#include <Arduino.h>
#include <stdio.h>
#include "RTC_variables.h"
#include "RTC_functions.h"

FILE uart_output = {0};         // clear the FILE struct at the beginning. This struct will be used for data stream

volatile uint8_t last_measure = 0;

void setup(){

    setup_RTC();
    setup_timer2_to_read_time_from_RTC();

    Serial.begin(115200);
    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);     // get the output stream and use UART (uart_putchar) , no flags , only for writing 
    stdout = &uart_output;                                                      // connect standart output stream with my struct FILE
    printf("Hello from printf!\n");

}


void loop(){

    if(last_measure != isr_timer2_counter){
        printf("isr_counter: %u\n", isr_timer2_counter);
    }
    last_measure = isr_timer2_counter;
}

ISR(TIMER2_OVF_vect){


    if(write_to_RTC){
        if(command_to_send & mask){
            PORTD |= _BV(DAT); // send 1 
        }
        else{
            PORTD &= ~_BV(DAT); // send 0
        }
        PORTD &= ~_BV(CLK);  // rising edge
        PORTD |= _BV(CLK);
    }


    else if (read_from_RTC){

        if(command_byte){               // send command byte 
            if(command_to_send & mask){
                PORTD |= _BV(DAT);      // 1
            }
            else{
                PORTD &= ~_BV(DAT);     // 0
            }

            PORTD |= _BV(CLK); // rising edge 

            if(delay_for_states){ // need to add delay beacuse LOW/HIGH states need some duration
                mask = mask << 1;
                send_bits++;
                
                if(send_bits < BYTE_LENGTH){
                    PORTD &= ~_BV(CLK);         // falling edge  **NEW**
                }
                else{
                    command_byte = false;
                    received_bits = 0;
                    received_command = 0;
                    mask = 0x01;

                    DDRD &= ~_BV(DAT);       // Data as input  **NEW**
                    PORTD &= ~_BV(DAT);     // Disable pull-up resistor
                }
            }
            delay_for_states = !delay_for_states;
        }

        else{
            if(received_bits < BYTE_LENGTH){
                PORTD &= ~_BV(CLK);     // falling edge

                if(delay_for_states){

                    if(PIND & (1 << DAT)){     // **NEW**
                    received_command |= (1 << received_bits);
                    }
                    else{
                        received_command &= ~(1 << received_bits);
                    } 
                    received_bits++;

                    PORTD |= _BV(CLK);  // rising edge 

                    if(received_bits == BYTE_LENGTH){
                        read_from_RTC = false;
                        PORTB &= ~_BV(RST);  // set RST to 0 

                        DDRD |= _BV(DAT);       // Data as output  **NEW**
                        PORTD &= ~_BV(DAT);     // Set 0 to Data
                    }
                }
                delay_for_states = !delay_for_states;
            }    
        }
    }

    else{   

        if(isr_timer2_counter == 255){      // update time every ~4 sec

            printf("ISR event\n");
            read_from_RTC = true;

            DDRD |= _BV(DAT);       // Data as output   **NEW**
            PORTD &= ~_BV(DAT);     // Set 0 to Data

            PORTD &= ~_BV(CLK); // CLK to 0
            PORTB |= _BV(RST);  // set RST to 1 

            command_to_send = 0x81; // read seconds
            send_bits = 0; 
            delay_for_states = false;

            isr_timer2_counter = 0;
        }
        else{
            isr_timer2_counter++;
        }
    }
}