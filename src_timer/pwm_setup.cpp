#include <Arduino.h>
#include "pwm_setup.h"


void pins_setup(void){

// Port B
DDRB = _BV(Big_Green_diode) | _BV(Big_Red_diode) | _BV(Big_Yellow_diode);     // all output 
PORTB = _BV(Big_Green_diode) | _BV(Big_Red_diode) | _BV(Big_Yellow_diode);    // all HIGH 

// Port D
DDRD = _BV(Small_Green_diode) | _BV(Small_Red_diode) | _BV(Small_Yellow_diode);     // all output 
PORTD = _BV(Small_Green_diode) | _BV(Small_Red_diode) | _BV(Small_Yellow_diode);    // all HIGH 

}


void timer0_setup(void){    // Fast PWM 

// 0. Reset registers 
TCCR0B = 0;
TCCR0A = 0;

// 1. Enable clock
TCCR0B |=  (1 << CS00);      //  1      001 - enable internal clock, prescaler = 8   
TCCR0B |=  (1 << CS01);      //  1      16\8 = 2 MHz 
TCCR0B &= ~(1 << CS02);      //  0      Period is 0.5 MHz

// 2. Counting mode 
TCCR0A |=  (1 << WGM00); // 1       110 - Fast PWM , set TOV Flag on TOP (OCR2A and OCR2B)
TCCR0A |=  (1 << WGM01); // 1       
TCCR0B &= ~(1 << WGM02); // 0

// 3. Select PWM mode 
TCCR0A &= ~(1 << COM0A0);  // 0  control OC2A pin behaviour -> non-inverting mode, change on OCR2A
TCCR0A |=  (1 << COM0A1);  // 1 

TCCR0A &= ~(1 << COM0B0);  // 0  control OC2B pin behaviour -> non-inverting mode, change on OCR2B
TCCR0A |=  (1 << COM0B1);  // 1 

OCR0A = 0;   // D6 - SmallGreenDiode
OCR0B = 64;   // D5 - SmallYellowDiode
}

void timer1_setup(void){

// 0. Reset registers 
TCCR1B = 0;
TCCR1A = 0;
TIMSK1 = 0;

// 1. Enable clock
TCCR1B |=  (1 << CS10);      //  1      001 - enable internal clock, prescaler = 1   
TCCR1B &= ~(1 << CS11);      //  0      16 MHz 
TCCR1B &= ~(1 << CS12);      //  0      Period is  1/16 MHz

// 2. Counting mode 
TCCR1A &= ~(1 << WGM10); // 0       0000 - Normal operation mode TOP - 0xFFFF , set TOV Flag on MAX (0xFFFF). OCR0A and OCR0B are disabled
TCCR1A &= ~(1 << WGM11); // 0
TCCR1B &= ~(1 << WGM12); // 0
TCCR1B &= ~(1 << WGM13); // 0

// 3. Enable interrupt on compare match 
TIMSK1 |= (1 << OCIE1B); 
TIMSK1 |= (1 << OCIE1A);
}


void timer2_setup(void){    // Fast PWM

// 0. Reset registers 
TCCR2B = 0;
TCCR2A = 0;
TIMSK2 = 0;

// 1. Enable clock
TCCR2B |=  (1 << CS20);      //  1      001 - enable internal clock, prescaler = 8   
TCCR2B |=  (1 << CS21);      //  1      16\8 = 2 MHz 
TCCR2B &= ~(1 << CS22);      //  0      Period is 0.5 MHz

// 2. Counting mode 
TCCR2A |=  (1 << WGM20); // 1       110 - Fast PWM , set TOV Flag on TOP (OCR2A and OCR2B)
TCCR2A |=  (1 << WGM21); // 1       
TCCR2B &= ~(1 << WGM22); // 0

// 3. Select PWM mode 
TCCR2A &= ~(1 << COM2A0);  // 0  control OC2A pin behaviour -> non-inverting mode, change on OCR2A
TCCR2A |=  (1 << COM2A1);  // 1 

TCCR2A &= ~(1 << COM2B0);  // 0  control OC2B pin behaviour -> non-inverting mode, change on OCR2B
TCCR2A |=  (1 << COM2B1);  // 1 

OCR2A = 128;   // D11 - BigGreenDiode
OCR2B = 192;   // D3  - SmallRedDiode
}






