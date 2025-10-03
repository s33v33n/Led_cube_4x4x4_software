#include <Arduino.h>
#include <stdio.h>
#include "effects.h"
#include "pins.h"
#include "functions.h"
#include "IR_functions.h"
#include "IR_variables.h"


enum Effect{

  None,
  Effect_0,
  Effect_1,
  Effect_2,
  Effect_3,
};

Effect current_effect = Effect_0;

struct Task
{
  uint16_t interval;
  void (*function)();
};

Task tasks[] = {

  {0, nullptr},
  {1000, effect0_scope::effect0},
  {250, effect1_scope::effect1},
  {250, effect2_scope::effect2},
  {2000, effect3_scope::effect3}
};

FILE uart_output = {0};         // clear the FILE struct at the beginning. This struct will be used for data stream


void setup(){

  led_cube_pins_setup();

  //======== temporary initial effect
  all_layers_low();
  delay(500);
  
  for(uint8_t i = 0; i < 4; i++){
    write_next_layer(i);
    delay(750);
  }
  //========

  Serial.begin(115200);
  while(!Serial);
  Serial.println("Serial ready\n");


  //======== IR setup 
  configure_timer1_for_IR_signal_measurement();
  set_IR_input_pin();

  //========


  //======== Serial.print change to printf
  fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);     // get the output stream and use UART (uart_putchar) , no flags , only for writing 

  stdout = &uart_output;                                                      // connect standart output stream with my struct FILE

  printf("Hello from printf!\n");
  printf("Hello from printf one more time!\n");
  //========

}


void handleCODE(){

  // int c;
  // if (Serial.available() > 0) {
  //     c = Serial.read();
  // }

  // switch(c){
  //   case '0': all_layers_low(); current_effect = None; break;
  //   case 'a': current_effect = Effect_0; break;
  //   case 'b': current_effect = Effect_1; break;
  //   case 'c': current_effect = Effect_2; break;
  //   case 'd': current_effect = Effect_3; break;
  // }

  if(received_bits == 32){

        // 1. disable interrputs
        TIMSK1 &= ~_BV(ICIE1);

        // 2. Calculate command 
        print_received_value_in_binary(received_value);

        uint8_t command = received_value >> 16;         // command 
        uint8_t neg_command = received_value >> 24;     // negative command 

        if(command == (uint8_t)(~neg_command)){
            printf("Correct command = !neg_command -> %u , %u\n" , command , neg_command);
        }

        switch(command){
            case BUTTON_0: printf("BUTTON_0\n"); all_layers_low(); current_effect = None; break;
            case BUTTON_1: current_effect = Effect_0; break;
            case BUTTON_2: current_effect = Effect_1; break;
            case BUTTON_3: current_effect = Effect_2; break;
            case BUTTON_4: current_effect = Effect_3; break;
            case BUTTON_5: printf("BUTTON_5\n"); break;
            default: printf("Nothing\n"); break;
        }
	    received_bits = 0;
        
        // 3. enable interrupts 
        TIMSK1 |= _BV(ICIE1);

        printf("command: %u \n" , command);
    }

}


uint32_t last_time = 0;

void loop(){

  handleCODE();

  Task *taskPtr = &tasks[current_effect];    

  if(millis() - last_time > taskPtr -> interval){

    if(taskPtr -> function){
      taskPtr -> function();
    }
    last_time = millis();
  }

}


ISR(TIMER1_CAPT_vect){      // vector address: 0x0014

    uint16_t actual_timer1 = ICR1;

    IR_signal_width = (actual_timer1 - last_timer1)/2;
    pulse_t pulse = pulse_command(IR_signal_width);

    switch (pulse) {
        case PULSE_SHORT:
            received_value = received_value >> 1;
            received_bits++;
            break;
        case PULSE_LONG:
            received_value = (received_value >> 1) | 0x80000000;
            received_bits++;
            break;
        case PULSE_13MS:
            received_value = 0;
            received_bits = 0;
            break;
        case PULSE_ERROR:
            received_bits = 0;
            break;
        default:
            received_bits = 0;
            break;
    }
    
    last_timer1 = actual_timer1;
}

