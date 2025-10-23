#include <Arduino.h>

// RTC setup 
void setup_RTC();
void setup_timer2_to_read_time_from_RTC();
void RTC_setup_starting_values();

// RTC reading time 
uint32_t read_time_from_DS1302();   // function to start reading time  
uint8_t read_time(uint8_t);         // function to read exact data (hour, minute etc.)
void print_time_in_BCD(uint32_t);

// RTC send command 
void write_command(uint16_t); // -> command_byte + command 

// times for states in RTC communication 
void state_time(void);
void RST_to_CLK_time(void);
void CLK_to_CE_time(void);