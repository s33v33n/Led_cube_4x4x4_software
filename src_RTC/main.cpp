#include <Arduino.h>
#include <stdio.h>
#include "RTC_variables.h"
#include "RTC_functions.h"

uint16_t read_time_from_DS1302(void);

FILE uart_output = {0};         // clear the FILE struct at the beginning. This struct will be used for data stream

void setup(){

    setup_RTC();
    setup_timer2_to_read_time_from_RTC();
    set_time(0x0080); // 0x0080   00000000 10000000 

    Serial.begin(115200);
    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);     // get the output stream and use UART (uart_putchar) , no flags , only for writing 
    stdout = &uart_output;                                                      // connect standart output stream with my struct FILE
    printf("Hello from printf!\n");
}




void loop(){

    // if(last_measure != isr_timer2_counter){
    //     printf("isr_counter: %u\n", isr_timer2_counter);
    // }
    // last_measure = isr_timer2_counter;


    if(read_time_DS1302){
        uint16_t read_time = read_time_from_DS1302();
        printf("Final time: %u\n", read_time);
        read_time_DS1302 = false;
    }
}

uint16_t read_time_from_DS1302(){

    // READ hours 
    uint8_t command = 0x85;             // 10000101 (to read hour from DS1302)
    uint8_t hour = read_time(command);
    //printf("hour: %u\n", hour);

    // READ minutes
    command = 0x83;             // 10000011 (to read minutes from DS1302)
    uint8_t minutes = read_time(command);
    //printf("minutes: %u\n", minutes);

    uint16_t time = 0;
    time = time | hour;
    printf("time | hour: %u\n", time);
    time = time << 8;
    printf("time << 8: %u\n", time);
    time |= minutes;
    printf("time |= minutes: %u\n", time);
    
    printf("minutes: %u\n", minutes);
    printf("hour: %u\n", hour);

    return time;     // time in BCD HH:MM
}


ISR(TIMER2_OVF_vect){ // overflow event = 0.016384

    if(isr_timer2_counter == 255){  // read time every ~ 4 sec
        read_time_DS1302 = true;
        isr_timer2_counter = 0;
    }
    else{
        isr_timer2_counter++;
    }
}