#ifndef RTC_H
#define RTC_H

#include <Arduino.h>

class RTC{
    public:
        void send_command_byte(byte addr);
        void write_minutes(byte min);
        void write_hour(byte hour);
};


#endif // !RTC_H
