#include <Arduino.h>
#include "RTC_functions.h"
#include "RTC_variables.h"

int uart_putchar(char c, FILE* f) { // ** get the chars and send them to printf , FILE* f is useless function fdev_setup_streem needs it **
    if (c == '\n') {
        Serial.write('\r');     // return to the beginning of the line -> CR carrige return  
    }                           // go to the next line -> LF line feed 
                                // ** first computers were made just like a writing machines, needed to do carrige return + get new line **  
    
    Serial.write(c);            // send the char     
    return 0;
}

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

uint32_t read_time_from_DS1302(){

    // READ hours 
    uint8_t command = 0x85;                 // 10000101 (to read hour from DS1302)
    uint8_t hour = read_time(command);
    //printf("hour: %u\n", hour);

    // READ minutes
    command = 0x83;                         // 10000011 (to read minutes from DS1302)
    uint8_t minutes = read_time(command);
    //printf("minutes: %u\n", minutes);

    // READ seconds
    command = 0x81;                         // 10000011 (to read seconds from DS1302)
    uint8_t seconds = read_time(command);
    //printf("minutes: %u\n", minutes);

    return ((hour << 16) | (minutes << 8) | seconds);     // time in BCD HH:MM:SS
}

uint8_t read_time(uint8_t command){

    // prepare pins
    DDRD |= _BV(DAT);       // Data as output  
    PORTD &= ~_BV(DAT);     // Set 0 to Data
    PORTD &= ~_BV(CLK);     // CLK to 0

    // start communication 
    PORTB |= _BV(RST);      // set RST to 1
    
    RST_to_CLK_time();

    uint8_t mask = 0x01; 

    for(int i=0; i < BYTE_LENGTH; i++){ // send command byte 

        // select state (0 or 1)
        if(command & mask){
            PORTD |= _BV(DAT);
        }
        else{
            PORTD &= ~_BV(DAT);
        }

        PORTD |= _BV(CLK);  // rising edge
        state_time();

        PORTD &= ~_BV(CLK);     // falling edge (if i=7 first data out is available)
        state_time();

        mask = mask << 1; 
    }
    DDRD &= ~_BV(DAT);   // Data as input   
    PORTD |= _BV(DAT);   // Enable pull-up resistor 

    uint8_t received_command = 0;
    for(uint8_t received_bits=0; received_bits < BYTE_LENGTH; received_bits++){

        PORTD |= _BV(CLK);  // rising edge 
        state_time();
        
        if(PIND & (1 << DAT)){
            received_command |= (1 << received_bits);
        }

        PORTD &= ~_BV(CLK);     // falling edge
        state_time();
    }

    PORTB &= ~_BV(RST);      // set RST to 0
    PORTD &= ~_BV(CLK);      // set CLK to 0

    return received_command;    // time in BCD 
}

void write_command(uint16_t command){

    // prepare pins
    DDRD |= _BV(DAT);       // Data as output  
    PORTD &= ~_BV(DAT);     // Set 0 to Data
    PORTD &= ~_BV(CLK);     // CLK to 0

    // start communication 
    PORTB |= _BV(RST);      // set RST to 1
    
    RST_to_CLK_time();

    uint16_t mask = 0x0001;
    for(int i=0; i < BYTE_LENGTH; i++){ // send command byte 

        // select state (0 or 1)
        if(command & mask){
            PORTD |= _BV(DAT);
        }
        else{
            PORTD &= ~_BV(DAT);
        }

        PORTD |= _BV(CLK);  // rising edge
        state_time();

        PORTD &= ~_BV(CLK);     // falling edge
        state_time();

        mask = mask << 1; 
    }

    for(int i=0; i < BYTE_LENGTH; i++){ // send command byte 

        // select state (0 or 1)
        if(command & mask){
            PORTD |= _BV(DAT);
        }
        else{
            PORTD &= ~_BV(DAT);
        }

        PORTD |= _BV(CLK);  // rising edge
        state_time();

        PORTD &= ~_BV(CLK);     // falling edge
        state_time();

        mask = mask << 1; 
    }
    
    PORTB &= ~_BV(RST);      // set RST to 0
    PORTD &= ~_BV(CLK);      // set CLK to 0
}


// system clock period is 62,5 ns
void state_time(void){ // wait 1125 ns 

    for(int i=0; i < 18; i++){ }
}

void RST_to_CLK_time(void){ // wait 4187,5 ns 

    for(int i=0; i < 67; i++){ }
}

void CLK_to_CE_time(void){ // wait 375 ns 

    for(int i=0; i < 6; i++){ }
}



