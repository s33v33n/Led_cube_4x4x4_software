#include <Arduino.h>
#include "effects.h"
#include "pins.h"
#include "functions.h"
#include "timers_setup.h"

#include <IRremote.hpp>
#include <TaskScheduler.h>

//// ISRs ////

volatile uint8_t value_effect_4 = 0;

volatile uint8_t factor_effect_5 = 1;
volatile int8_t value_effect_5 = 0;

//// TaskScheduler ////
Scheduler runner;

Task E0(1000, TASK_FOREVER, &effect0_scope::effect0, &runner, false);
Task E1(250, TASK_FOREVER, &effect1_scope::effect1, &runner, false);
Task E2(250, TASK_FOREVER, &effect2_scope::effect2, &runner, false);
Task E3(2000, TASK_FOREVER, &effect3_scope::effect3, &runner, false);
Task E4(1000, TASK_ONCE, &effect4_scope::effect4, &runner, false);
Task E5(1000, TASK_ONCE, &effect5_scope::effect5, &runner, false); 

void setup()
{
  led_cube_pins_setup();

  Serial.begin(115200);
  while (!Serial); // wait untill Serial is ready 
  
  //initial_effect(); // Initial effect - 3 times turn on and off 
  
  runner.startNow();

  runner.addTask(E0);
  runner.addTask(E1);
  runner.addTask(E2);
  runner.addTask(E3);
  runner.addTask(E4);
  runner.addTask(E5);

  //======== temporary initial effect
  all_layers_low();
  delay(750);
  
  for(uint8_t i = 0; i < 4; i++){
    write_next_layer(i);
    delay(750);
  }
  //========

  // E4.enable();

  // OR 
  
  //timer0_pwm_on();
  //timer2_pwm_on();
}

void loop(){ 

  runner.execute();
}

// ISR(TIMER0_OVF_vect){ // efect 4

//   value_effect_4++;
//   if (value_effect_4 == 10){
//     value_effect_4 = 0;
//     OCR0B++;
//     OCR0A++;  
//     OCR2B++;
//     OCR2A++;    
//   }
// }


ISR(TIMER2_OVF_vect){ // efect 5

  value_effect_5++;
  if (value_effect_5 == 10){

    if(OCR0A == 255){
      factor_effect_5 = factor_effect_5 * (-1);
    }

    value_effect_5 = 0;
    OCR0B += factor_effect_5;
    OCR0A += factor_effect_5; 
    OCR2B += factor_effect_5;
    OCR2A += factor_effect_5;   
  }
}

