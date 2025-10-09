#include <Arduino.h>
#include "RTC_functions.h"
#include "RTC_variables.h"

void setup_RTC(){

    // PORT B
    DDRB |= _BV(RST);       // Reset as output 
    PORTB &= ~_BV(RST);     // Set 0 to Reset     

    // PORT D 
    DDRD |= _BV(DAT);       // Data as output 
    PORTD &= ~_BV(DAT);     // Set 0 to Data     

    DDRD |= _BV(CLK);       // Clock as output 
    PORTD &= ~_BV(CLK);     // Set 0 to Clock
}

void setup_timer2_to_read_time_from_RTC(){

    // Read every ~minute in ISR time from RTC -> setup clock to have that ISR 

    TCCR2A = 0; // normal operation counts 0 -> 255
    TCCR2B = 0;
    TCCR2B |= _BV(CS20) | _BV(CS21) | _BV(CS22); // set prescaler to 1024 
    
    TIMSK2 = 0;
    TIMSK2 |= _BV(TOIE2);   // enable overflow interrupt 
}

int uart_putchar(char c, FILE* f) { // ** get the chars and send them to printf , FILE* f is useless function fdev_setup_streem needs it **
    if (c == '\n') {
        Serial.write('\r');     // return to the beginning of the line -> CR carrige return  
    }                           // go to the next line -> LF line feed 
                                // ** first computers were made just like a writing machines, needed to do carrige return + get new line **  
    
    Serial.write(c);            // send the char     
    return 0;
}

uint8_t read_time(uint8_t command){

    DDRD |= _BV(DAT);       // Data as output  
    PORTD &= ~_BV(DAT);     // Set 0 to Data

    PORTD &= ~_BV(CLK);     // CLK to 0
    PORTB |= _BV(RST);      // set RST to 1 

    uint8_t mask = 0x01;

    for(int i=0; i < BYTE_LENGTH; i++){ // send command byte 

        PORTD &= ~_BV(CLK);     // falling edge
        if(command & mask){
            PORTD |= _BV(DAT);
        }
        else{
            PORTD &= ~_BV(DAT);
        }
        PORTD |= _BV(CLK);  // rising edge
        
        mask = mask << 1; 
    }

    uint8_t received_command = 0;

    DDRD &= ~_BV(DAT);   // Data as input  
    PORTD &= ~_BV(DAT);   // Disable pull-up resistor 

    for(uint8_t received_bits=0; received_bits < BYTE_LENGTH; received_bits++){

        PORTD &= ~_BV(CLK);     // falling edge
        if(PIND & (1 << DAT)){
            received_command |= (1 << received_bits);
        }
        PORTD |= _BV(CLK);  // rising edge 
        //printf("received_command: %u\n", received_command);
    }

    PORTB &= ~_BV(RST);      // set RST to 0
    PORTD &= ~_BV(CLK);      // set CLK to 0

    return received_command;    // time in BCD 
}

void set_time(uint16_t command){

    DDRD |= _BV(DAT);       // Data as output  
    PORTD &= ~_BV(DAT);     // Set 0 to Data

    PORTD &= ~_BV(CLK);     // CLK to 0
    PORTB |= _BV(RST);      // set RST to 1 

    uint8_t mask = 0x01;

    for(int i=0; i < 16; i++){ // send command byte 

        PORTD &= ~_BV(CLK);     // falling edge
        if(command & mask){
            PORTD |= _BV(DAT);
        }
        else{
            PORTD &= ~_BV(DAT);
        }
        PORTD |= _BV(CLK);  // rising edge
        
        mask = mask << 1; 
    }
    
    PORTB &= ~_BV(RST);      // set RST to 0
    PORTD &= ~_BV(CLK);      // set CLK to 0
}