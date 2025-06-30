/*
1. Set the clock source 
In register TCCR0B setup the CS02:0 bits. This register's address for writing is 0x45

2. Set the counting mode 
In register TCCR0A setup the WGM01:0 bits and in register TCCR0B setup WGM02 bit.

3. Enable the compare match mode 
In register TIMSK0 set bits OCIE0B and OCIE0A to enable output compare mode interrupt

*/

#include <Arduino.h>
#include <pwm_setup.h>

volatile uint8_t value_A = 0;
volatile uint8_t value_B = 0;


void setup() {
cli();            //stop interrupts for till we make the settings 

pins_setup();     //setup all pins (6 LEDs to test PWM)
timer0_setup();   //setup timer0 (Fast PWM)
timer1_setup();   //setup timer1 (Timer to control things)
timer2_setup();   //setup timer2 (Fast PWM)


Serial.begin(115200);
while (!Serial); // wait untill Serial is ready 
  
sei();           //Enable back the interrupts   
}

void loop() {

Serial.print("value_A = ");
Serial.print(value_A);
Serial.print(", OCR0A = ");
Serial.print(OCR0A);
Serial.print(", OCR2A = ");
Serial.print(OCR2A);
Serial.print(", value_B = ");
Serial.print(value_B);
Serial.print(", OCR0B = ");
Serial.print(OCR0B);
Serial.print(", OCR2B = ");
Serial.println(OCR2B);

}


ISR(TIMER1_COMPA_vect){ // ISR period is 16 ms  

value_A++;
  if (value_A == 10){
    value_A = 0;
    OCR0A++;
    OCR2A++;

    // if(OCR2A == 255){
    //   OCR0A = 0;
    //   OCR2A = 0;
    //   //PORTD ^= 0x20;
    // }
    
  }
}

ISR(TIMER1_COMPB_vect){

value_B++;
  if (value_B == 10){
    value_B = 0;
    OCR0B++;
    OCR2B++;
    
    // if (OCR2B == 255){
    //   OCR0B = 0;
    //   OCR2B = 0;
    //   //PORTD ^= 0x40;
    // }
  }

}



// ISR(TIMER0_COMPA_vect){ // ISR period is 16 ms  

// value_A++;
//   if (value_A == 10){
//     value_A = 0;
//     OCR2B++;
//     if(OCR2B == 255){
//       OCR2B = 0;
//       PORTD ^= 0x20;
//     }
    
//   }
// }

// ISR(TIMER0_COMPB_vect){

// value_B++;

//   if (value_B == 10){
//     OCR2A++;
//     value_B = 0;
//     if (OCR2A == 255){
//       PORTD ^= 0x40;
//       OCR2A = 0;
//     }
//   }

// }