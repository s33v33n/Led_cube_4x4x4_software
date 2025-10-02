#include <Arduino.h>
#include <stdio.h>
#include "IR_functions.h"

#define MAX_IR_SIGNALS 33 // first 13,5 ms burst + 32 bits
#define WRONG_DECODED -1 


volatile uint16_t IR_signal_width = 0;
volatile uint16_t last_timer1 = 0;
volatile uint16_t IR_signals_counter = 0;

volatile bool first_edge = true;

typedef enum{

    // Due to the AVR timers limits, recorded time differs from pulses time in NEC protocol, repeating code is not used (not required + save memory) 

    PULSE_SHORT,        // logic 0 
    PULSE_LONG,         // logic 1
    PULSE_13MS,         // first in sequence (9 + 4,5 ms) 
    PULSE_ERROR,        // error while receiving 

    //PULSE_11MS,         // repeat code 
}pulse_t;

volatile pulse_t IR_signals_values[MAX_IR_SIGNALS];

int decode_IR_signals(volatile pulse_t *IR_signals_values);


pulse_t pulse_command(uint16_t time){

    if( 870 <= time && time <= 1370){
        return PULSE_SHORT;
    }
    else if(2000 <= time && time <= 2500){
        return PULSE_LONG;
    }
    else if(13250 <= time && time <= 13750){
        return PULSE_13MS;
    }
    else{
        return PULSE_ERROR;
    }
}


int uart_putchar(char c, FILE* f) { // ** get the chars and send them to printf , FILE* f is useless function fdev_setup_streem needs it **
    if (c == '\n') {
        Serial.write('\r');     // return to the beginning of the line -> CR carrige return  
    }                           // go to the next line -> LF line feed 
                                // ** first computers were made just like a writing machines, needed to do carrige return + get new line **  
    
    Serial.write(c);            // send the char     
    return 0;
}

FILE uart_output = {0};         // clear the FILE struct at the beginning. This struct will be used for data stream

void setup() {
    configure_timer1_for_IR_signal_measurement();
    set_IR_input_pin();

    Serial.begin(115200);

    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);     // get the output stream and use UART (uart_putchar) , no flags , only for writing 

    stdout = &uart_output;                                                      // connect standart output stream with my struct FILE

    printf("Hello from printf!\n");
    printf("Hello from printf one more time!\n");

}

void loop() {

    if(IR_signals_counter == MAX_IR_SIGNALS){

        // 1. disable interrputs
        TIMSK1 &= ~_BV(ICIE1);
        
        // 2. calculate signals 
        int received_command = decode_IR_signals(IR_signals_values);

        for(int i=0; i < MAX_IR_SIGNALS; i++){
            printf("%d signal: %d\n", i, IR_signals_values[i]);
        }

        if(received_command > 0 ){
            first_edge = true;
            IR_signals_counter = 0;
            printf("received_command: %d\n", received_command);

        }
        else{

            printf("received_command: %d\n", received_command);
        }


        // 3. enable interrupts 
        TIMSK1 |= _BV(ICIE1);

        while (1)
        {
            /* code */
        }
        

    }
}

int decode_IR_signals(volatile pulse_t *IR_signals_values){

    uint16_t decoded = 0;
    
    if(IR_signals_values[0] == PULSE_13MS){

        for(int i = 17; i < MAX_IR_SIGNALS; i++){
            if(IR_signals_values[i] == PULSE_SHORT){
                decoded = (0 >> decoded);
            }
            else if (IR_signals_values[i] == PULSE_LONG){
                decoded = (1 >> decoded);
            }
            else{
                return WRONG_DECODED;
            }

            printf("Decoded: %d\n", decoded);
        }

        uint8_t command = decoded | 0xFF;
        uint8_t negative_command = (decoded >> 8) | 0xFF;

        printf("command: %u , neg_command: %u\n", command, negative_command);

        if(command == (uint8_t)(~negative_command)){
            return command;
        }
        else{
            return WRONG_DECODED;
        }

    }
    else{
        return WRONG_DECODED;
    }


}


ISR(TIMER1_CAPT_vect){      // vector address: 0x0014

    uint16_t actual_timer1 = ICR1;

    if(first_edge){

        first_edge = false;
    }
    else{

        IR_signal_width = (actual_timer1 - last_timer1)/2;
        IR_signals_values[IR_signals_counter++] = pulse_command(IR_signal_width);
    }
    
    last_timer1 = actual_timer1;
}

