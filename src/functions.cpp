#include <Arduino.h>
#include "pins.h"


void columns_turn_on(void){
    PORTB &= ~_BV(LATCH); // close latch 
    PORTB &= ~_BV(RESET); // reset register set to LOW 
    PORTB |= _BV(RESET); // reset register set to HIGH --> all registers(Qa-Qh) set to LOW --> all columns give light 
    PORTB |= _BV(LATCH); // open latch 
    PORTB &= ~_BV(LATCH); // close latch
  }
  
void columns_turn_off(void){
    PORTB &= ~_BV(LATCH); // close latch
    PORTD |= _BV(SER); // SER --> HIGH.

    for(int i=0; i < 16; i++){
        PORTD &= ~_BV(CLK); // clk low    *rising edge*
        PORTD |= _BV(CLK);  // clk high 
    }

    PORTD &= ~_BV(SER); // SER --> LOW.
    PORTB |= _BV(LATCH); // open latch 
    PORTB &= ~_BV(LATCH); // close latch
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

    PORTD |= _BV(SER); // SER -> HIGH 
    for(int i=0; i < 16; i++){ // initialize leds columns one by one
                
      if(columns_states[Columns - i]){   // if column's index is high send 1 to turn off this column 
        PORTD &= ~_BV(SER); // SER -> LOW
        PORTD &= ~_BV(CLK); // rising edge 
        PORTD |= _BV(CLK);
        PORTD |= _BV(SER); // SER -> HIGH  
      }
  
      else{
        PORTD &= ~_BV(CLK); // rising edge 
        PORTD |= _BV(CLK);
      }

    }
    PORTB &= ~_BV(LATCH); // latch results for each column 
    PORTB |= _BV(LATCH);

    memset(columns_states, 0, sizeof(columns_states)); // write 0 to all columns index
  }
    
}
  
void all_layers_low(void){
    PORTB &= ~_BV(layer1) & ~_BV(layer3) & ~_BV(layer4);
    PORTD &= ~_BV(layer2);
}
  
 
void write_next_layer(uint8_t next){
  
    switch(next){
  
      case 0:
        all_layers_low();
        PORTB |= _BV(layer1);
      break;
  
      case 1:
        all_layers_low();
        PORTD |= _BV(layer2);
      break;
  
      case 2:
        all_layers_low();
        PORTB |= _BV(layer3);
      break;
  
      case 3:
        all_layers_low();
        PORTB |= _BV(layer4);
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

  PORTD |= _BV(SER); // SER -> HIGH

  for(uint8_t i=0; i<Columns + 1; i++){
    if(i==column){
      PORTD &= ~_BV(SER); // SER -> LOW
      PORTD &= ~_BV(CLK); // rising edge 
      PORTD |= _BV(CLK);
      PORTD |= _BV(SER); // SER -> HIGH
      Serial.println("HIGH: " + String(i));
    }
    else{
      PORTD &= ~_BV(CLK); // rising edge 
      PORTD |= _BV(CLK);
      Serial.println("LOW: " + String(i));
    }
  } 
  PORTB &= ~_BV(LATCH); // latch results for each column 
  PORTB |= _BV(LATCH);

  uint8_t layer = column / 4;

  write_next_layer(layer);

  Serial.println("Layer: " + String(layer) + "  Column: " + String(column)); 
}


void fountian(uint8_t *random_array, uint8_t states, uint8_t layer){

  writing_cols_states::write_selected_cols_states(random_array, states, true);
  write_next_layer(layer);
}





/////// OLD IMPLEMENTATION ////////

/*

void columns_turn_on(){
  PORTB &= ~_BV(LATCH); // close latch 
  PORTB &= ~_BV(RESET); // reset register set to LOW 
  PORTB |= _BV(RESET); // reset register set to HIGH --> all registers(Qa-Qh) set to LOW --> all columns give light 
  PORTB |= _BV(LATCH); // open latch 
  PORTB &= ~_BV(LATCH); // close latch
}

void columns_turn_off(){
  PORTB &= ~_BV(LATCH); // close latch
  PORTD |= _BV(SER); // SER --> HIGH.

  for(int i=0; i < columns; i++){
    PORTD &= ~_BV(CLK); // clk low    *rising edge*
    PORTD |= _BV(CLK);  // clk high 
  }

  PORTD &= ~_BV(SER); // SER --> LOW.
  PORTB |= _BV(LATCH); // open latch 
  PORTB &= ~_BV(LATCH); // close latch
}

void gen_cols_random_numbers(){
  last_index = 15;
  while(last_index >= 0){ // shuffle numbers 
    randomIndex = random(0, last_index);
    temp = random_numbers_0_15[last_index];
    random_numbers_0_15[last_index] = random_numbers_0_15[randomIndex];
    random_numbers_0_15[randomIndex] = temp;
    last_index--;
  }
}

void all_cols_index_low(){
  for(int i=0; i < columns; i++){ // turn off all columns index 
    column_states[i] = false;
  }
}

void all_cols_index_high(){
  for(int i=0; i < columns; i++){ // turn off all columns index 
    column_states[i] = true;
  }
}

void all_layers_low(){
  PORTB &= ~_BV(layer1) & ~_BV(layer3) & ~_BV(layer4);
  PORTD &= ~_BV(layer2);
}

void write_columns_states(int states_to_write, bool random){

  if(random){
    for(int i=0; i< states_to_write; i++){
      column_states[random_numbers_0_15[i]] = true; // random numbers arry has random number --> this random number is random index --> for column index to be turned on 
    }
  }
  else{
    for(int i=0; i< states_to_write; i++){
      column_states[numbers_0_15[i]] = true; 
    }
  }
  

  PORTD |= _BV(SER); // SER -> HIGH 
  for(int i=1; i < columns + 1; i++){ // initialize leds columns one by one
              
    if(column_states[columns - i]){   // if column's index is high send 1 to turn off this column 
      PORTD &= ~_BV(SER); // SER -> LOW
      PORTD &= ~_BV(CLK); // rising edge 
      PORTD |= _BV(CLK);
      PORTD |= _BV(SER); // SER -> HIGH 
    }

    else{
      PORTD &= ~_BV(CLK); // rising edge 
      PORTD |= _BV(CLK);
    }
  }
  PORTB &= ~_BV(LATCH); // latch results for each column 
  PORTB |= _BV(LATCH);

}

void write_selected_columns_states(int *array_states){

  int states = sizeof(array_states)/sizeof(array_states[0]);

  for(int i=0; i< states; i++){
    int columnt_to_write = array_states[i];
    column_states[numbers_0_15[columnt_to_write]] = true; 
  }


}

void write_next_layer(int next){

  switch(next){

    case 0:
      all_layers_low();
      PORTB |= _BV(layer1);
    break;

    case 1:
      all_layers_low();
      PORTD |= _BV(layer2);
    break;

    case 2:
      all_layers_low();
      PORTB |= _BV(layer3);
    break;

    case 3:
      all_layers_low();
      PORTB |= _BV(layer4);
    break;

    default:
      all_layers_low();
    break;
  }

}*/

//PORTD |= _BV(SER); // SER --> HIGH.  (HIGH state deactivates columns) 

  // uint switch_time = millis();  
  // while(last_index > 0){                // Initial effect (turning off all columns one by one)
  //   if (millis() - switch_time > 125){
      
  //     PORTD &= ~_BV(CLK); // rising edge 
  //     PORTD |= _BV(CLK);
           
  //     PORTB &= ~_BV(LATCH); // close latch after each column turned off 
  //     PORTB |= _BV(LATCH);
 
  //     last_index--;
  //     switch_time = millis();
  //   }
  // }

  // gen_cols_random_numbers();
  
