#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include "RTC_variables.h"
#include "RTC_functions.h"

FILE uart_output = {0};         // clear the FILE struct at the beginning. This struct will be used for data stream

void setup(){

    Serial.begin(115200);
    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);     // get the output stream and use UART (uart_putchar) , no flags , only for writing 
    stdout = &uart_output;                                                      // connect standart output stream with my struct FILE
    printf("Hello from printf!\n");

    setup_RTC();
    setup_timer2_to_read_time_from_RTC();
    RTC_setup_starting_values();
}


void loop(){

    if(read_time_DS1302){
        uint32_t read_time = read_time_from_DS1302();       // time in BCD
        print_time_in_BCD(read_time);
        read_time_DS1302 = false;
    }
}


ISR(TIMER2_OVF_vect){ // overflow event = 0.016384

    if(isr_timer2_counter == 62){  // read time every ~ 4 sec (255)
        read_time_DS1302 = true;
        isr_timer2_counter = 0;
    }
    else{
        isr_timer2_counter++;
    }
}