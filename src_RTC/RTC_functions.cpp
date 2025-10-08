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