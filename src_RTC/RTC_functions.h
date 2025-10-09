#include <Arduino.h>

void setup_RTC();
void setup_timer2_to_read_time_from_RTC();
int uart_putchar(char, FILE*);

uint8_t read_time(uint8_t);
void set_time(uint16_t);