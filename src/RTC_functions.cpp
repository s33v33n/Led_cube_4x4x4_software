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
    TIMSK2 &= ~_BV(TOIE2);   // for start disable overflow interrupt 
}


void RTC_setup_starting_values(void){

    // To read properly data from RTC CH flag must be written to 0 and WP bit must be cleared

    //1. Clear WP bit (7-bit (MSB))
    uint16_t clear_WP_bit = 0x008E; 
    write_command(clear_WP_bit);
    printf("Cleared WP bit\n");

    //2. Set hours (00), 24-time domain, am 
    // uint16_t set_hours = 0x0084;
    // write_command(set_hours);
    // printf("Set hours\n");

    //3. Set minutes (59)
    uint16_t set_minutes = 0x5982;
    write_command(set_minutes);
    printf("Set minutes\n");

    //4. Set seconds (50) and clear flag        // (not now)CH flag is still 1 
    uint16_t set_seconds = 0x5080;
    write_command(set_seconds);
    printf("Set seconds\n");
    

    //5. Clear CH flag 
    // uint16_t clear_CH_flag = 0x0080;
    // write_command(clear_CH_flag);
    // printf("Cleared CH flag\n");

}

uint32_t read_time_from_DS1302(){

    // READ hours 
    uint8_t command = 0x85;                 // 10000101 (to read hour from DS1302)
    uint8_t hour = read_time(command);

    // READ minutes
    command = 0x83;                         // 10000011 (to read minutes from DS1302)
    uint8_t minutes = read_time(command);

    // READ seconds
    command = 0x81;                         // 10000001 (to read seconds from DS1302)
    uint8_t seconds = read_time(command);

    return ((uint32_t)hour << 16) | ((uint32_t)minutes << 8) | seconds;     // time in BCD HH:MM:SS
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


void print_time_in_BCD(uint32_t time){

    printf("Final time in BCD (HH:MM:SS):  ");
    uint32_t mask = 0x800000;
    for(int i=0; i < 24; i++){
        
        if((i==8) || (i==16)){
            printf("    ");
        }

        if(time & mask){
            printf("1");
        }
        else{
            printf("0");
        }
        mask = mask >> 1;
    }
    printf("\n");
}


// system clock period is 62,5 ns
void state_time(void){ // wait 1125 ns 

    for(int i=0; i < 18; i++){ }
}

void RST_to_CLK_time(void){ // wait 4187,5 ns 

    for(int i=0; i < 67; i++){ }
}

void CLK_to_CE_time(void){ // wait 375 ns   (actual not used)

    for(int i=0; i < 6; i++){ }
}



