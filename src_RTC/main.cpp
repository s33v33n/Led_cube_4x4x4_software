#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include "RTC_variables.h"
#include "RTC_functions.h"

char *convert_time_BCD_to_string(uint16_t);

FILE uart_output = {0};         // clear the FILE struct at the beginning. This struct will be used for data stream

void setup(){

    Serial.begin(115200);
    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);     // get the output stream and use UART (uart_putchar) , no flags , only for writing 
    stdout = &uart_output;                                                      // connect standart output stream with my struct FILE
    printf("Hello from printf!\n");

    setup_RTC();
    setup_timer2_to_read_time_from_RTC();

    //1. Clear WP bit (7-bit (MSB))
    uint16_t clear_WP_bit = 0x008E; 
    write_command(clear_WP_bit);
    printf("Cleared WP bit\n");

    //2. Set hours (00), 24-time domain, am 
    // uint16_t set_hours = 0x0084;
    // write_command(set_hours);
    // printf("Set hours\n");

    // //3. Set minutes (00)
    // uint16_t set_minutes = 0x0082;
    // write_command(set_minutes);
    // printf("Set minutes\n");

    // //4. Set seconds (00) CH flag is still 1 
    // uint16_t set_seconds = 0x8080;
    // write_command(set_seconds);
    // printf("Set seconds\n");
    

    //5. Clear CH flag 
    // uint16_t clear_CH_flag = 0x0080;
    // write_command(clear_CH_flag);
    // printf("Cleared CH flag\n");

    for(int i=0; i < 15000; i++){

    }
    

    uint8_t seconds = read_time(read_commands::READ_SECONDS);
    printf("Read seconds: %u\n", seconds);
    
    uint8_t minutes = read_time(read_commands::READ_MINUTES);
    printf("Read minutes: %u\n", minutes);
    
    uint8_t hours = read_time(read_commands::READ_HOURS);
    printf("Read hours: %u\n", hours);
}




void loop(){

    if(read_time_DS1302){
        uint32_t read_time = read_time_from_DS1302();       // time in BCD

        printf("Final time in BCD (HH:MM:SS) : ");
        uint32_t mask = 0x800000;
        for(int i=0; i < 24; i++){
            
            if((i==8) || (i==16)){
                printf("    ");
            }

            if(read_time & mask){
                printf("1");
            }
            else{
                printf("0");
            }
            mask = mask >> 1;
        }
        printf("\n");

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