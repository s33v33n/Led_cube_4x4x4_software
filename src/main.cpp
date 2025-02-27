#include <Arduino.h>
#include <IRremote.hpp>
#include <TaskScheduler.h>

///// REGISTER B /////
#define layer1 1    // Pin 9  = Port B bit 1
#define LATCH  4    // Pin 12 = Port B bit 4
#define layer3 3    // Pin 11 = Port B bit 3
#define layer4 2    // Pin 10 = Port B bit 2 
#define RESET 0     // Pin 8  = Port B bit 0   wyzwalany stanem niskim 

///// REGISRER D /////
#define SER 7       // Pin 7  = Port D bit 7
#define layer2 3    // Pin 3  = Port D bit 3
#define CLK 4       // Pin 4  = Port D bit 4
#define IR_input 2  // Pin 2  = Port D bit 2

///// FUNCTIONS /////
void columns_turn_off();
void columns_turn_on();
void switch_case(int);
void gen_cols_random_numbers();
void all_cols_index_low();
void write_columns_states();
void all_layers_low();
void write_next_layer(int);

///// COLUMNS /////
int numbers_0_15[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int random_numbers_0_15[16] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};
int columns = 16;
int last_index = columns; 
int temp = 0;
int randomIndex = 0;
int what_number = 0;
bool column_states[16] = {0};
int j=0;
int next_layer = 0;
int cols_turned_on = 0;

///// TIMER /////
unsigned long long switch_time = 0;

///// IR MODULE /////
int IR_code;

  
void setup()
{
  // PORT B 
  DDRB = _BV(layer1) | _BV(LATCH) | _BV(layer3) | _BV(layer4) | _BV(RESET); // 4 - layer1 , 3 - LATCH , 2 - layer3 , 1 - layer4 , 0 - RESET 
  PORTB = _BV(layer1) | _BV(layer3) | _BV(layer4) | _BV(RESET);// 1 - layer1 , 0 - LATCH , 1 - layer3 , 1 - layer4 , 1 - RESET
  
  //PORT D
  DDRD = _BV(SER) | _BV(layer2) | _BV(CLK); //  7 - SER , 6 - layer2 , 4 - CLK , 2 - IR_input
  PORTD = _BV(layer2); // 0 - SER , 1 - layer2 , 0 - CLK , pull up resistor disable - IR_input

  Serial.begin(9600);
  while (!Serial); // wait untill Serial is ready 
  
  columns_turn_on(); // all columns set to HIGH. (GND activates columns) 

  PORTD |= _BV(SER); // SER --> HIGH.  (HIGH state deactivates columns) 

  switch_time = millis();  
  while(last_index > 0){                // Initial effect (turning off all columns one by one)
    if (millis() - switch_time > 125){
      
      PORTD &= ~_BV(CLK); // rising edge 
      PORTD |= _BV(CLK);
           
      PORTB &= ~_BV(LATCH); // close latch after each column turned off 
      PORTB |= _BV(LATCH);

      last_index--;
      switch_time = millis();
    }
  }

  gen_cols_random_numbers();
  columns_turn_off();
}

void loop(){ 

  /*
    1. How many drops?
    2. 
  
  */

  int drops_number = random(1,10);
  //int *drops = new int[drops_number];

  all_cols_index_low(); // all columns have low index 
  gen_cols_random_numbers(); // random numbers are generated 

  for(int i=0; i< drops_number; i++){
    column_states[random_numbers_0_15[i]] = true; // random numbers arry has random number --> this random number is random index --> for column index to be turned on 
  }
  write_columns_states();

  switch_time = millis();
  while(true){
    if (millis() - switch_time > 250){
      write_next_layer(next_layer);
      
      next_layer++;
      if(next_layer > 3){
        next_layer = 0;
        drops_number = random(1,10);
        all_cols_index_low(); // all columns have low index 
        gen_cols_random_numbers(); // random numbers are generated 

        for(int i=0; i< drops_number; i++){
          column_states[random_numbers_0_15[i]] = true; // random numbers arry has random number --> this random number is random index --> for column index to be turned on 
        }
        write_columns_states();

      }
      switch_time = millis();
      Serial.println("what_number: " + String(next_layer));
    }

  }



  //delete [] drops;

  // int drops;
  //   gen_cols_random_numbers(); // generate random number, so random array is ready to use 
  //   all_cols_index_low(); 
    
  //   for(int i=0; i< sizeof(drops)/sizeof(drops[0]); i++){
  //     drops[i] = random_numbers_0_15[i];
  //     column_states[drops[i]] = true;
  //   }

  //   write_columns_states();
  //   all_layers_low();
  //   switch_time = millis();
  //   while(true){
  //     if (millis() - switch_time > 250){
  //       write_next_layer(next_layer);
        
  //       next_layer++;
  //       if(next_layer > 3){
  //         next_layer = 0;
  //         gen_cols_random_numbers();
  //         all_cols_index_low(); 
  //         for(int i=0; i< sizeof(drops)/sizeof(drops[0]); i++){
  //           drops[i] = random_numbers_0_15[i];
  //           column_states[drops[i]] = true;
  //         }
  //         write_columns_states();
  //       }
  //       switch_time = millis();
  //       Serial.println("what_number: " + String(next_layer));
  //     }

  //   }

  
    
}



void switch_case (int c)
{
  int cols_turned_on = 0;
  int i=0;
  switch_time = millis();

  switch (c){

    case 0: // random turn on all columns (GOOD)
      
    if (millis() - switch_time > 1000){ // interval 1s 

      what_number = random_numbers_0_15[cols_turned_on]; // get random index for column_states
      column_states[what_number] = true; // turn on this column 
  
      write_columns_states(); // write states to states arrys -> tells which columns shoud be turned on 
    
      cols_turned_on++; // number of iterations 
      
      if(cols_turned_on > columns ){
  
        columns_turn_off();
        all_cols_index_low();
        gen_cols_random_numbers(); // generate new random columns
  
        cols_turned_on = 0;  
      }
  
      switch_time = millis();
      Serial.println("what_number: " + String(what_number) + ", cols_turned_on: " + String(cols_turned_on));
    }
        
    break;  

    case 1: // rain 

    

    break;
    
    default:
      columns_turn_off();
    break;   
  }   
}

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

void write_columns_states(){

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


}