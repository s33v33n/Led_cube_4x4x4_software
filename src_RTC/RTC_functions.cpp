/*
All time data must be delivered in BCD foramt - 4 bits for every digit 

*/

#include <Arduino.h>
#include "RTC.h"
#include "nano.h"

void RTC::send_command_byte(byte addr){

    PORTC |= (1 << RST); // reset to HIGH

    for(uint8_t i=0; i < 8; i++){

        PORTD &= ~(1 << SCLK); // falling edge 

        if(addr&0x01){ // HIGH state 
            PORTD |= (1 << DATA);
        }

        else{ // LOW state
            PORTD &= ~(1 << DATA);
        }

        PORTD |= (1 << SCLK); // rising egde 

        addr = addr >> 1; // next bit 
    }
    PORTC &= ~(1 << RST); // reset to LOW
}

void RTC::write_minutes(byte min){

    PORTC |= (1 << RST); // reset to HIGH

    RTC::send_command_byte(0x80); // addres to set seconds

    // set seconds as number 00 (only 7 bits)
    for(uint8_t i=0; i < 7; i++){

        PORTD &= ~(1 << SCLK); // falling edge

        PORTD &= ~(1 << DATA); // load 0 to bit 
        
        PORTD |= (1 << SCLK); // rising egde  
    }
    PORTC &= ~(1 << RST); // reset to LOW


    PORTC |= (1 << RST); // reset to HIGH
    RTC::send_command_byte(0x82); // addres to set minutes
    
    // set minutes - given by user 
    for(uint8_t i=0; i < 7; i++){

        PORTD &= ~(1 << SCLK); // falling edge 

        if(min&0x01){ // HIGH state 
            PORTD |= (1 << DATA);
        }

        else{ // LOW state
            PORTD &= ~(1 << DATA);
        }

        PORTD |= (1 << SCLK); // rising egde 
        min = min >> 1; // next bit 
    }
    PORTC &= ~(1 << RST); // reset to LOW
}

void RTC::write_hour(byte hour){

    PORTC |= (1 << RST); // reset to HIGH
    hour = 0x3F & hour; // set hour to 0-24 format 7th and 6th bits must be 0
    RTC::send_command_byte(0x84); // addres to set hours

    // set hours - given by user  
    for(uint8_t i=0; i < 8; i++){

        PORTD &= ~(1 << SCLK); // falling edge 

        if(hour&0x01){ // HIGH state 
            PORTD |= (1 << DATA);
        }

        else{ // LOW state
            PORTD &= ~(1 << DATA);
        }

        PORTD |= (1 << SCLK); // rising egde 
        hour = hour >> 1; // next bit 
    }

    PORTC &= ~(1 << RST); // reset to LOW
}