// #include <Arduino.h>


// Port B
#define Big_Green_diode     3   // Pin 11 = Port B bit 3 
#define Big_Yellow_diode    2   // Pin 10 = Port B bit 2 
#define Big_Red_diode       1   // Pin 9 = Port B bit 1 

// Port D
#define Small_Green_diode   6   // Pin 6 = Port D bit 6   OC0A
#define Small_Yellow_diode  5   // Pin 5 = Port D bit 5   OC0B
#define Small_Red_diode     3   // Pin 3 = Port D bit 3


void pins_setup(void);

// void timer0_setup(void);
// void timer1_setup(void);
// void timer2_setup(void);


#include <Arduino.h>

void timer0_pwm_on(void);
void timer0_ISR_effect4_on(void);
void timer0_pwm_off(void);

void timer2_pwm_on(void);
void timer2_ISR_effect5_on(void);
void timer2_pwm_off(void);


// void timer1_setup(void);
// void timer1_turn_on_ISR(void);
// void timer1_turn_off_ISR(void);

