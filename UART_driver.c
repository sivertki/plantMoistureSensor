#include <avr/io.h>
#include <stdio.h>
#include "UART_driver.h"

void UART_init(void) {
    //set baud rate
    UBRR0H = (unsigned char) (MYUBRR >> 8);
    UBRR0L = (unsigned char) MYUBRR;

    //enable receiver and transmitter
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    //set frame format
    UCSR0C = (1<<USBS0) | (1<<UCSZ00);

    //enable IO-stream
    fdevopen(UART_transmit, UART_receive);
}

void UART_transmit(unsigned char data) {
    //wait for empty tramsmit buffer
    while( !(UCSR0A & (1<<UDRE0) ));

    //Put data into buffer, sends the data
    UDR0 = data;
}

unsigned char UART_receive() {
    //Wait for data to be received
    while( !(UCSR0A & (1<<RXC0)) );

    //Get and return received data from buffer
    return UDR0;
}