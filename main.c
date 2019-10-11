#define F_CPU 16000000

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

#include "UART_driver.h"

int main(void) {

    
    DDRB = 0x01;
    PORTB |= (1 << PB0);
    
    UART_init();


    while(1) {
        unsigned char received = UART_receive();
        printf(received);
        PORTB |= (1 << PB0);
        
        _delay_ms(1000);

        printf("\n\r");
        PORTB &= ~(1 << PB0);
        //_delay_ms(1000);
    }

}