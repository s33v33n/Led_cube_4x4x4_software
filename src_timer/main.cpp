/*
1. Set the clock source 
In register TCCR0B setup the CS02:0 bits. This register's address for writing is 0x45

2. Set the counting mode 
In register TCCR0A setup the WGM01:0 bits and in register TCCR0B setup WGM02 bit.

3. Enable the compare match mode 
In register TIMSK0 set bits OCIE0B and OCIE0A to enable output compare mode interrupt

*/

#include <Arduino.h>
#include "pwm_setup.h"

volatile uint8_t value_effect_4 = 0;

volatile uint8_t factor_effect_5 = 1;
volatile int8_t value_effect_5 = 0;

volatile bool effect5_flag = false;
volatile bool effect4_flag = true;


void setup() {
//cli();            //stop interrupts for till we make the settings 

// timer0_pwm_on();
// timer2_pwm_on();
// timer0_ISR_effect4_on();

pins_setup();

// timer0_pwm_on();
// timer2_pwm_on();
// timer0_ISR_effect4_on();



timer0_pwm_on();
timer2_pwm_on();
//timer2_ISR_effect5_on();

  OCR2B = 0;      // D3  - LAYER_1
  OCR0B = 64;     // D5  - LAYER_2
  OCR0A = 128;    // D6  - LAYER_3
  OCR2A = 192;    // D11 - LAYER_4



Serial.begin(115200);
while (!Serial); // wait untill Serial is ready 
  
//sei();           //Enable back the interrupts   
}

void loop() {

Serial.print("effect5_flag = ");
Serial.print(effect5_flag);
Serial.print(", effect4_flag = ");
Serial.print(effect4_flag);
Serial.print(", OCR0A = ");
Serial.print(OCR0A);
Serial.print(", OCR0B = ");
Serial.print(OCR0B);
Serial.print(", OCR2A = ");
Serial.print(OCR2A);
Serial.print(", OCR2B = ");
Serial.println(OCR2B);

}




ISR(TIMER2_OVF_vect){ // efect 5

  if(effect5_flag){
  value_effect_5++;
  
    if (value_effect_5 == 10){

      value_effect_5 = 0;

      if(OCR0A == 255){
        factor_effect_5 = -1;
      }
      else if (OCR0A == 0)
      {
        factor_effect_5 = 1;
      }

      OCR0B += factor_effect_5;
      OCR0A += factor_effect_5; 
      OCR2B += factor_effect_5;
      OCR2A += factor_effect_5;   
    }
  }


  if(effect4_flag){
  value_effect_4++;
    
    if (value_effect_4 == 10){
      value_effect_4 = 0;
      OCR0B++;
      OCR0A++;  
      OCR2B++;
      OCR2A++;    
    }
  }
}


