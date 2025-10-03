#include <Arduino.h>
#include <pins.h>
#include "IR_functions.h"

void configure_timer1_for_IR_signal_measurement(void){

    // need normal operation 
    TCCR1A = 0;
    TCCR1B = 0;

    // configure prescaler 
    TCCR1B |= _BV(CS11); // 010 prescaler is 8. 16MHz/8 = 2MHz 

    // edge select
    TCCR1B &= ~_BV(ICES1); // first is neagtive edge (0 written)

    // enable ISR (interrupts)
    TIMSK1 |= _BV(ICIE1);
}

void set_IR_input_pin(void){

    DDRB &= ~_BV(IR_RECEIVER);  // set as input 
    PORTB &= ~_BV(IR_RECEIVER); //  disable pull-up resistor 
}

void print_received_value_in_binary(uint32_t received_value){

    uint32_t mask = 0x80000000;
    
    for(int i=0; i < 32; i++){

        if(received_value & mask){
            printf("1");
        }
        else{
            printf("0");
        }
        mask = mask >> 1;
    }
    printf("\n");

}


pulse_t pulse_command(uint16_t time){

    if( 620 <= time && time <= 1620){
        return PULSE_SHORT;
    }
    else if(1750 <= time && time <= 2750){
        return PULSE_LONG;
    }
    else if(13000 <= time && time <= 14000){
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