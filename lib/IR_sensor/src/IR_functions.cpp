#include <Arduino.h>
#include <pins.h>
#include <IR_functions.h>
#include <variables.h>
#include <effects.h>
#include <functions.h>

void configure_timer1_for_IR_signal_measurement(void){

    // need normal operation 
    TCCR1A = 0;
    TCCR1B = 0;

    // configure prescaler 
    TCCR1B |= _BV(CS11); // 010 prescaler is 8. 16MHz/8 = 2MHz 

    // edge select
    TCCR1B &= ~_BV(ICES1); // first is neagtive edge (0 written)

    // enable ISR (interrupts)
    TIMSK1 |= _BV(ICIE1);
}

void set_IR_input_pin(void){

    DDRB &= ~_BV(IR_RECEIVER);  // set as input 
    PORTB &= ~_BV(IR_RECEIVER); //  disable pull-up resistor 
}

void print_received_value_in_binary(uint32_t received_value){

    uint32_t mask = 0x80000000;
    
    for(int i=0; i < 32; i++){

        if(received_value & mask){
            printf("1");
        }
        else{
            printf("0");
        }
        mask = mask >> 1;
    }
    printf("\n");

}


pulse_t pulse_command(uint16_t time){

    if( 620 <= time && time <= 1620){
        return PULSE_SHORT;
    }
    else if(1750 <= time && time <= 2750){
        return PULSE_LONG;
    }
    else if(13000 <= time && time <= 14000){
        return PULSE_13MS;
    }
    else{
        return PULSE_ERROR;
    }
}

void handleCODE(){

  if(received_bits == 32){

    // 1. disable interrputs
    TIMSK1 &= ~_BV(ICIE1);

    //print_received_value_in_binary(received_value);

    // 2. Calculate command 
    uint8_t command = received_value >> 16;         // command 
    uint8_t neg_command = received_value >> 24;     // negative command 

    if(command == (uint8_t)(~neg_command)){
      //printf("Correct command = !neg_command -> %u , %u\n" , command , neg_command);
  
      switch(command){
        case BUTTON_0: printf("BUTTON_0, command: %u\n", command); all_layers_low(); current_effect = None; TIMSK2 &= ~_BV(TOIE2); break;
        case BUTTON_1: printf("BUTTON_1, command: %u\n", command); current_effect = Effect_0; TIMSK2 &= ~_BV(TOIE2); break;
        case BUTTON_2: printf("BUTTON_2, command: %u\n", command); current_effect = Effect_1; TIMSK2 &= ~_BV(TOIE2); break;
        case BUTTON_3: printf("BUTTON_3, command: %u\n", command); current_effect = Effect_2; TIMSK2 &= ~_BV(TOIE2); break;
        case BUTTON_4: printf("BUTTON_4, command: %u\n", command); current_effect = Effect_3; TIMSK2 &= ~_BV(TOIE2); break;
        case BUTTON_5: printf("BUTTON_5, command: %u\n", command); current_effect = Effect_4; TIMSK2 &= ~_BV(TOIE2); break;
        default: printf("Nothing\n"); TIMSK2 &= ~_BV(TOIE2); break;
      }

      received_bits = 0;
    }

    // 3. enable interrupts 
    TIMSK1 |= _BV(ICIE1);
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