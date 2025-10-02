#include <Arduino.h>
#include <pins.h>

void configure_timer1_for_IR_signal_measurement(void){

    // need normal operation 
    TCCR1A = 0;
    TCCR1B = 0;

    // configure prescaler 
    TCCR1B |= _BV(CS11); // 010 prescaler is 8. 16MHz/8 = 2MHz 

    // edge select
    TCCR1B &= ~_BV(ICES1); // first is neagtive edge (0 written)

    // enable ISR (interrupts)
    TIMSK1 |= _BV(ICIE1);
}

void set_IR_input_pin(void){

    DDRB &= ~_BV(IR_RECEIVER);  // set as input 
    PORTB &= ~_BV(IR_RECEIVER); //  disable pull-up resistor 
}