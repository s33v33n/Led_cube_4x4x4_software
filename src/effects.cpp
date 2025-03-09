#include <Arduino.h>
#include "functions.h"
#include "pins.h"

namespace effect0_scope {

    // void effect0(void){ // random turn on all columns 
  
    //   int cols_turned_on = 0; 
    //   bool random_flag = false;
  
    //   write_columns_states(cols_turned_on, random_flag); // write states to states arrys -> tells which columns shoud be turned on 
      
    
    //   cols_turned_on++; // number of iterations 
      
    //   if(cols_turned_on > columns + 1){
  
    //     columns_turn_off();
    //     all_cols_index_low();
    //     gen_cols_random_numbers(); // generate new random columns
  
    //     cols_turned_on = 0;  
    //   }
  
    //   Serial.println(" cols_turned_on: " + String(cols_turned_on));
    // }

  uint8_t columns_turn_on = 0;
  uint8_t *random_numbers = random_numbers_generator::return_random_numbers_0_15();
  bool random_falg = true;

  void effect0(void){

    if(columns_turn_on > Columns + 1){

      delete [] random_numbers;

      random_numbers = random_numbers_generator::return_random_numbers_0_15();

      columns_turn_on = 0;    
    }

    writing_cols_states::write_selected_cols_states(random_numbers, columns_turn_on, random_falg);

    columns_turn_on++;

    Serial.println("Number " + String(columns_turn_on) + ":" + String(random_numbers[columns_turn_on]));
  }
}

namespace effect1_scope { // Rain 

  // int drops_number = random(numbers_0_15[0],numbers_0_15[15] + 1); // drops range 0-16
  // bool random_flag = false;
  // int next_layer = 4;

  // void effect1(void){

  //   write_next_layer(next_layer);
      
  //   next_layer++;
  //   if(next_layer > 3){
    
  //     all_cols_index_low(); // all columns have low index 
  //     gen_cols_random_numbers(); // random numbers are generated 

  //     drops_number = random(numbers_0_15[0],numbers_0_15[15]);
  //     write_columns_states(drops_number, random_flag);

  //     next_layer = 0;
  //   }
  //   Serial.println("what_number: " + String(next_layer));

  // }

  uint8_t next_layer = 4;
  uint8_t *random_numbers;
  uint8_t drops_number = 0;
  bool random_flag = true;

  void effect1(void){
    
    if(next_layer > 3){

      delete [] random_numbers;

      drops_number = random(1, Columns + 1);

      random_numbers = random_numbers_generator::return_random_numbers_0_15();
      
      writing_cols_states::write_selected_cols_states(random_numbers, drops_number, random_flag);

      next_layer = 0;
    }

    write_next_layer(next_layer);
    next_layer++;  
  }  
}

namespace effect2_scope{  // Fountain 

  uint8_t *random_numbers = random_numbers_generator::return_random_numbers_0_15();
  uint8_t states = 0;

  int8_t next_layer = 0;
  int8_t layer_factor = 1;

  void effect2(void){

    if(next_layer < 0){
      next_layer = 0;
      layer_factor = 1;
      delete [] random_numbers;
      random_numbers = random_numbers_generator::return_random_numbers_0_15();
      states = 4;
    }

    else if (next_layer > 3){
      next_layer = 3;
      layer_factor = -1;
      states = 12;
    }

    fountian(random_numbers, states, next_layer);
  
    next_layer += layer_factor;
  }

}

namespace effect3_scope {

  uint8_t *random_numbers = random_numbers_generator::return_random_numbers_0_15();
  uint8_t passed_drops = 0; 

  void effect3(void){

    passed_drops++;
    
    if(passed_drops > Columns){

      delete [] random_numbers;
      random_numbers = random_numbers_generator::return_random_numbers_0_15();
      passed_drops = 0;
    }
    
    turn_one_led(random_numbers[passed_drops]);
    
    Serial.println("Passed drops: " + String(random_numbers[passed_drops]));
  }
}
  