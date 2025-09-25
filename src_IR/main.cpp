#include <Arduino.h>
#include <stdio.h>

int uart_putchar(char c, FILE* f) { // ** get the chars and send them to printf , FILE* f is useless function fdev_setup_streem needs it **
    if (c == '\n') {
        Serial.write('\r');     // return to the beginning of the line -> CR carrige return  
    }                           // go to the next line -> LF line feed 
                                // ** first computers were made just like a writing machines, needed to do carrige return + get new line **  
    
    Serial.write(c);            // send the char     
    return 0;
}

FILE uart_output = {0};         // clear the FILE struct at the beginning. This struct will be used for data stream

void setup() {
    Serial.begin(115200);

    fdev_setup_stream(&uart_output, uart_putchar, NULL, _FDEV_SETUP_WRITE);     // get the output stream and use UART (uart_putchar) , no flags , only for writing 

    stdout = &uart_output;                                                      // connect standart output stream with my struct FILE

    printf("Hello from printf!\n");
    printf("Hello from printf one more time!");

}

void loop() {
    
}
