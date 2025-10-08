#include <Arduino.h>

#define RST 5           // pin 13 = port B pin 5
#define DAT 7           // pin 7 = port D pin 7
#define CLK 4           // pin 4 = port D pin 4 
#define BYTE_LENGTH 8

extern volatile uint8_t isr_timer2_counter;
extern volatile bool write_to_RTC;
extern volatile bool read_from_RTC;
extern volatile uint8_t mask;

extern volatile bool command_byte;
extern volatile bool delay_for_states; 

extern volatile uint8_t send_bits;
extern volatile uint8_t command_to_send;

extern volatile uint8_t received_bits;
extern volatile uint8_t received_command;