/* Secure for multidefinition dht11. If sensor library is attached somewhere else, preprocessor will ignore second, third, etc.. attachment
#ifndef -> if not defined (first timer) -> #define -> #endif
#ifndef -> already defined -> #endif */

#ifndef DHT11_H
#define DHT11_H

#include <Arduino.h>


class dht11{
    public:
        int read(void);
        int humidity;
        int temperature;
        
};

#endif 