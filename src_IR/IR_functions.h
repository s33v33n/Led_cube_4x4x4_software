#include <Arduino.h>

void configure_timer1_for_IR_signal_measurement(void);
void set_IR_input_pin(void);

void print_received_value_in_binary(uint32_t);

typedef enum{

    // Due to the AVR timers limits, recorded time differs from pulses time in NEC protocol, repeating code is not used (not required + save memory) 

    PULSE_SHORT,        // logic 0 
    PULSE_LONG,         // logic 1
    PULSE_13MS,         // first in sequence (9 + 4,5 ms) 
    PULSE_ERROR,        // error while receiving 

    //PULSE_11MS,         // repeat code 
}pulse_t;

pulse_t pulse_command(uint16_t);

int uart_putchar(char, FILE*);