#include <Arduino.h>
#include "pins.h"


void led_cube_pins_setup(void){

  //PORT D
  DDRD = _BV(LAYER_1) | _BV(LAYER_2) | _BV(LAYER_3) | _BV(CLK_CUBE); //  out - LAYER_1 , out - LAYER_2 , out - LAYER_3 ,  out - CLK_CUBE
  PORTD = _BV(LAYER_1) | _BV(LAYER_2) | _BV(LAYER_3); // high - LAYER_1 , high - LAYER_2 , high - LAYER_3 ,  low - CLK_CUBE

  // PORT B 
  DDRB = _BV(SER_CUBE) | _BV(LAYER_4) | _BV(RESET_CUBE) | _BV(LATCH_CUBE); // out - SER_CUBE , out - LAYER_4 , out - RESET_CUBE , out - LATCH_CUBE 
  PORTB = _BV(LAYER_1) | _BV(LAYER_3) | _BV(LAYER_4) | _BV(RESET_CUBE);// low - SER_CUBE , high - LAYER_4 , high - RESET_CUBE , low - LATCH_CUBE
}

void columns_turn_on(void){
  PORTB &= ~_BV(LATCH_CUBE); // close latch 
  PORTB &= ~_BV(RESET_CUBE); // reset register set to LOW 
  PORTB |= _BV(RESET_CUBE); // reset register set to HIGH --> all registers(Qa-Qh) set to LOW --> all columns give light 
  PORTB |= _BV(LATCH_CUBE); // open latch 
  PORTB &= ~_BV(LATCH_CUBE); // close latch
}
  
void columns_turn_off(void){
  PORTB &= ~_BV(LATCH_CUBE); // close latch
  PORTB |= _BV(SER_CUBE); // SER_CUBE --> HIGH.

  for(int i=0; i < 16; i++){
    PORTD &= ~_BV(CLK_CUBE); // clk low    *rising edge*
    PORTD |= _BV(CLK_CUBE);  // clk high 
  }

  PORTB &= ~_BV(SER_CUBE); // SER_CUBE --> LOW.
  PORTB |= _BV(LATCH_CUBE); // open latch 
  PORTB &= ~_BV(LATCH_CUBE); // close latch
}


void initial_effect(void){
  columns_turn_on(); 
  delay(500);
  columns_turn_off();
  delay(500);
  columns_turn_on();
  delay(500);
  columns_turn_off();
  delay(500);
  columns_turn_on();
  delay(500);
  columns_turn_off();
}

  
namespace random_numbers_generator {

  uint8_t numbers_array[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
  uint8_t randomIndex = 0;
  uint8_t temp = 0;
  int8_t last_index = 0;

  uint8_t *return_random_numbers_0_15(void){
    last_index = 15;
    while(last_index >= 0){ // shuffle numbers 
      randomIndex = random(0, last_index);
      temp = numbers_array[last_index];
      numbers_array[last_index] = numbers_array[randomIndex];
      numbers_array[randomIndex] = temp;
      last_index--;
    }

    uint8_t *ReadyArry = new uint8_t[16];
    
    for(int i=0; i < 16; i++){
      ReadyArry[i] = numbers_array[i];
    }

    return ReadyArry;
 }
}

namespace writing_cols_states{

  bool random = true;
  bool columns_states[16] = {0};

  void write_selected_cols_states(uint8_t *random_array, uint8_t states, bool random){

    if(random){
      for(int i=0; i < states; i++){
        columns_states[random_array[i]] = true;
      }
    }

    else{
      for(int i=0; i < states; i++){
        columns_states[i] = true;
      } 
    }

    PORTD |= _BV(SER_CUBE); // SER_CUBE -> HIGH 
    for(int i=0; i < 16; i++){ // initialize leds columns one by one
                
      if(columns_states[Columns - i]){   // if column's index is high send 1 to turn off this column 
        PORTB &= ~_BV(SER_CUBE); // SER_CUBE -> LOW
        PORTD &= ~_BV(CLK_CUBE); // rising edge 
        PORTD |= _BV(CLK_CUBE);
        PORTB |= _BV(SER_CUBE); // SER_CUBE -> HIGH  
      }
  
      else{
        PORTD &= ~_BV(CLK_CUBE); // rising edge 
        PORTD |= _BV(CLK_CUBE);
      }

    }
    PORTB &= ~_BV(LATCH_CUBE); // latch results for each column 
    PORTB |= _BV(LATCH_CUBE);

    memset(columns_states, 0, sizeof(columns_states)); // write 0 to all columns index
  }
    
}
  

void all_layers_low(void){
  PORTD &= ~_BV(LAYER_1) & ~_BV(LAYER_2) & ~_BV(LAYER_3);
  PORTB &= ~_BV(LAYER_4);
}


void all_layers_high(void){
  PORTD |= _BV(LAYER_1) | _BV(LAYER_2) | _BV(LAYER_3);
  PORTB |= _BV(LAYER_4);
}
  

void write_next_layer(uint8_t next){
  
    switch(next){
  
      case 0:
        all_layers_low();
        PORTD |= _BV(LAYER_1);
      break;
  
      case 1:
        all_layers_low();
        PORTD |= _BV(LAYER_2);
      break;
  
      case 2:
        all_layers_low();
        PORTD |= _BV(LAYER_3);
      break;
  
      case 3:
        all_layers_low();
        PORTB |= _BV(LAYER_4);
      break;
  
      default:
        all_layers_low();
      break;
    }
}

// void turn_on_one_column(uint8_t col){
    
  

// }

void turn_one_led (uint8_t column){

  columns_turn_off();

  PORTD |= _BV(SER_CUBE); // SER_CUBE -> HIGH

  for(uint8_t i=0; i<Columns + 1; i++){
    if(i==column){
      PORTB &= ~_BV(SER_CUBE); // SER_CUBE -> LOW
      PORTD &= ~_BV(CLK_CUBE); // rising edge 
      PORTD |= _BV(CLK_CUBE);
      PORTB |= _BV(SER_CUBE); // SER_CUBE -> HIGH
      //Serial.println("HIGH: " + String(i));
    }
    else{
      PORTD &= ~_BV(CLK_CUBE); // rising edge 
      PORTD |= _BV(CLK_CUBE);
      //Serial.println("LOW: " + String(i));
    }
  } 
  PORTB &= ~_BV(LATCH_CUBE); // latch results for each column 
  PORTB |= _BV(LATCH_CUBE);

  uint8_t layer = column / 4;

  write_next_layer(layer);

  //Serial.println("Layer: " + String(layer) + "  Column: " + String(column)); 
}


void fountian(uint8_t *random_array, uint8_t states, uint8_t layer){

  writing_cols_states::write_selected_cols_states(random_array, states, true);
  write_next_layer(layer);
}
