#include <Arduino.h>

void setup_RTC();
void setup_timer2_to_read_time_from_RTC();
int uart_putchar(char, FILE*);

uint8_t read_time(uint8_t);


uint32_t read_time_from_DS1302();
char *convert_time_BCD_to_string(uint16_t time);



// Send commands 

void write_command(uint16_t); // -> command_byte + command 



// times

void state_time(void);

void RST_to_CLK_time(void);

void CLK_to_CE_time(void);