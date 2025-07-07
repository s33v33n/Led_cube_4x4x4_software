#include <Arduino.h>
#include "effects.h"
#include "pins.h"
#include "functions.h"
#include "timers_setup.h"

#include <IRremote.hpp>
#include <TaskScheduler.h>

//// ISRs ////
volatile uint8_t value_A = 0;
volatile uint8_t value_B = 0;

//// TaskScheduler ////
Scheduler runner;

Task E0(1000, TASK_FOREVER, &effect0_scope::effect0, &runner, false);
Task E1(250, TASK_FOREVER, &effect1_scope::effect1, &runner, false);
Task E2(250, TASK_FOREVER, &effect2_scope::effect2, &runner, false);
Task E3(2000, TASK_FOREVER, &effect3_scope::effect3, &runner, false);
Task E4(1000, TASK_ONCE, &effect4_scope::effect4, &runner, false);
  
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

  //======== temporary initial effect
  all_layers_low();
  delay(1000);
  
  for(uint8_t i = 0; i < 4; i++){
    write_next_layer(i);
    delay(1000);
  }
  //========

  timer1_setup(); // setup timer1 (16-bit timer
  
  // timer0_pwm_off();
  // timer2_pwm_off();
  // timer1_turn_off_ISR();
  // E4.enable();
  
  timer1_turn_on_ISR();
  timer2_pwm_on();
  timer0_pwm_on();

}

void loop(){ 

  runner.execute();

}

ISR(TIMER1_COMPA_vect){ // ISR period is 16 ms  

value_A++;
  if (value_A == 10){
    value_A = 0;
    OCR0A++;
    OCR2A++;    
  }
}

ISR(TIMER1_COMPB_vect){

value_B++;
  if (value_B == 10){
    value_B = 0;
    OCR0B++;
    OCR2B++;
  }
}
