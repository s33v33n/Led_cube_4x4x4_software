#include <Arduino.h>

void timer0_pwm_on(void){    // Fast PWM 

    // 0. Reset registers 
    TCCR0B = 0;
    TCCR0A = 0;
    TIMSK0 = 0;

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

    OCR0B = 0;//64;     // D5 - LAYER_2
    OCR0A = 0;//128;      // D6 - LAYER_3

    // 4. Enable ISR 
    // TIMSK0 |=  (1 << TOIE0);    // Enable ISR on timeroverflow 
    // TIMSK0 &= ~(1 << OCIE0A);   // Disable ISR on COMPA
    // TIMSK0 &= ~(1 << OCIE0B);   // Disable ISR on COMPB
    
}

void timer0_ISR_effect4_on(void){
    
    // 4. Enable ISR 
    TIMSK0 |=  (1 << TOIE0);    // Enable ISR on timeroverflow 
    TIMSK0 &= ~(1 << OCIE0A);   // Disable ISR on COMPA
    TIMSK0 &= ~(1 << OCIE0B);   // Disable ISR on COMPB
}

void timer0_pwm_off(void){
    
    // 0. Reset registers 
    TCCR0B = 0;
    TCCR0A = 0;
    TIMSK0 = 0;
}


void timer2_pwm_on(void){    // Fast PWM

    // 0. Reset registers 
    TCCR2B = 0;
    TCCR2A = 0;
    TIMSK2 = 0;

    // 1. Enable clock
    TCCR2B |=  (1 << CS20);      //  1      111 - enable internal clock, prescaler = 1024   
    TCCR2B |=  (1 << CS21);      //  1      16 MHz / 1024 = 15,626 kHz 
    TCCR2B |=  (1 << CS22);      //  1      Period is 64 us

    // 2. Counting mode 
    TCCR2A |=  (1 << WGM20); // 1       110 - Fast PWM , set TOV Flag on TOP (OCR2A and OCR2B)
    TCCR2A |=  (1 << WGM21); // 1       
    TCCR2B &= ~(1 << WGM22); // 0

    // 3. Select PWM mode 
    TCCR2A &= ~(1 << COM2A0);  // 0  control OC2A pin behaviour -> non-inverting mode, change on OCR2A
    TCCR2A |=  (1 << COM2A1);  // 1 

    TCCR2A &= ~(1 << COM2B0);  // 0  control OC2B pin behaviour -> non-inverting mode, change on OCR2B
    TCCR2A |=  (1 << COM2B1);  // 1 

    // 4. Enable ISR 
    // TIMSK2 |=  (1 << TOIE2);    // Enable ISR on timeroverflow 
    // TIMSK2 &= ~(1 << OCIE0A);   // Disable ISR on COMPA
    // TIMSK2 &= ~(1 << OCIE0B);   // Disable ISR on COMPB
    
    OCR2B = 0;//0;      // D3  - LAYER_1
    OCR2A = 0;//192;   // D11 - LAYER_4
}

void timer2_ISR_effect5_on(){
    
    // Enable ISR
    TIMSK2 |=  (1 << TOIE2);    // Enable ISR on timeroverflow 
    TIMSK2 &= ~(1 << OCIE2A);   // Disable ISR on COMPA
    TIMSK2 &= ~(1 << OCIE2B);   // Disable ISR on COMPB
    
}

void timer2_pwm_off(void){

    // 0. Reset registers 
    TCCR2B = 0;
    TCCR2A = 0;
    TIMSK2 = 0;
}


// void timer1_setup(void){

//     // 0. Reset registers 
//     TCCR1B = 0;
//     TCCR1A = 0;
//     TIMSK1 = 0;

//     // 1. Enable clock
//     TCCR1B |=  (1 << CS10);      //  1      001 - enable internal clock, prescaler = 1   
//     TCCR1B &= ~(1 << CS11);      //  0      16 MHz 
//     TCCR1B &= ~(1 << CS12);      //  0      Period is  1/16 MHz

//     // 2. Counting mode 
//     TCCR1A &= ~(1 << WGM10); // 0       0000 - Normal operation mode TOP - 0xFFFF , set TOV Flag on MAX (0xFFFF). OCR0A and OCR0B are disabled
//     TCCR1A &= ~(1 << WGM11); // 0
//     TCCR1B &= ~(1 << WGM12); // 0
//     TCCR1B &= ~(1 << WGM13); // 0

//     // 3. Enable interrupt on compare match 
//     //TIMSK1 |= (1 << OCIE1B); 
//     //TIMSK1 |= (1 << OCIE1A);
// }

// void timer1_turn_on_ISR(void){
//     // 3. Enable interrupt on compare match 
//     TIMSK1 |= (1 << OCIE1B); 
//     TIMSK1 |= (1 << OCIE1A);
// }

// void timer1_turn_off_ISR(void){
//     // 3. Disable interrupt on compare match 
//     TIMSK1 &= ~(1 << OCIE1B); 
//     TIMSK1 &= ~(1 << OCIE1A);
// }
