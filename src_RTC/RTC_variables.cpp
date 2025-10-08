#include <Arduino.h>

volatile uint8_t isr_timer2_counter = 0;
volatile bool write_to_RTC = false;
volatile bool read_from_RTC = false;
volatile uint8_t mask = 0x01;

volatile bool command_byte = false;
volatile bool delay_for_states = false;

volatile uint8_t send_bits = 0;
volatile uint8_t command_to_send = 0;

volatile uint8_t received_bits = 0;
volatile uint8_t received_command = 0;