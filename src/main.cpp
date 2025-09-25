#include <Arduino.h>
#include "effects.h"
#include "pins.h"
#include "functions.h"
#include "timers_setup.h"


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

}


void handleCODE(){

  int c;
  if (Serial.available() > 0) {
      c = Serial.read();
  }

  switch(c){
    case '0': all_layers_low(); current_effect = None; break;
    case 'a': current_effect = Effect_0; break;
    case 'b': current_effect = Effect_1; break;
    case 'c': current_effect = Effect_2; break;
    case 'd': current_effect = Effect_3; break;
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
