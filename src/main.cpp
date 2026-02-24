#include <Arduino.h>
#include <stdio.h>

#include <effects.h>
#include <functions.h>
#include <IR_functions.h>
#include <RTC_functions.h>
#include <variables.h>
#include <pins.h>


FILE uart_output = {0};         // clear the FILE struct at the beginning. This struct will be used for data stream

void setup(){

  //======== Serial.print change to printf
  Serial.begin(115200);
  fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);     // get the output stream and use UART (uart_putchar) , no flags , only for writing 
  stdout = &uart_output;                                                      // connect standart output stream with my struct FILE
  printf("Hello from printf!\n");
  //========


  led_cube_pins_setup();

  //======== IR setup 
  configure_timer1_for_IR_signal_measurement();
  set_IR_input_pin();
  //========


  //======== RTC setup 
  setup_RTC();
  setup_timer2_to_read_time_from_RTC();
  RTC_setup_starting_values();
  //========

  initial_effect();
}



void loop(){

  // received code from IR sensor
  handleCODE();

  // select task (effect) do to 
  Task *taskPtr = &tasks[current_effect];    

  // do task with it's period
  if(millis() - last_time > taskPtr -> interval){

    if(taskPtr -> function){
      taskPtr -> function();
    }
    last_time = millis();
  }
}
