#include <Arduino.h>

void nano_setup(byte rst, byte sclk, byte data){

    // PORT D (digital)
    DDRD |= _BV(sclk) | _BV(data);      // pins as output
    PORTD |= _BV(sclk) | _BV(data); // starting as HIGH  
    
    // PORT C (analog)
    DDRC |= _BV(rst);       // pin as output 
    PORTC |= _BV(rst);      // starting as HIGH  

}

void data_pin_as_input(byte pin){
    
    DDRD &= ~_BV(pin); // pin as input
    PORTD |= _BV(pin); // pull-up enable
}

void data_pin_as_output(byte pin){
    
    DDRD |= _BV(pin);   // pin as output
    PORTD &= ~_BV(pin); // set LOW state 
}