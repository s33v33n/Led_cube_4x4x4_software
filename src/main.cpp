#include <Arduino.h>
#include "effects.h"
#include "pins.h"
#include "functions.h"

#include <IRremote.hpp>
#include <TaskScheduler.h>


//// TaskScheduler ////
Scheduler runner;

Task E0(1000, TASK_FOREVER, &effect0_scope::effect0, &runner, false);
Task E1(250, TASK_FOREVER, &effect1_scope::effect1, &runner, false);
Task E2(250, TASK_FOREVER,&effect2_scope::effect2, &runner, false);
Task E3(2000, TASK_FOREVER, &effect3_scope::effect3, &runner, false);
  
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
  
  initial_effect(); // Initial effect - 3 times turn on and off 
  
  runner.startNow();

  //runner.addTask(E0);
  E3.enable();
}

void loop(){ 

  runner.execute();

}

