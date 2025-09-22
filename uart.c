#define F_CPU 4915200
#define UBRR 31 //using 2400 as baudrate, we get this for the registers according to the formula in the datasheet
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

#define BIT_TIME_US (1000000 / BAUDRATE)


int uart_send_byte(char data,FILE* file){
    //Wait until the serial port is ready for sending something new
    while (!(UCSR0A & (1<<UDRE0)))
    ;
    //load new data into sending buffer
    UDR0 = data;
    return 0;
}

int uart_receive_byte (FILE* file){
    while (!(UCSR0A & (1<<RXC0)))
    ;
    //load new data into sending buffer
    return UDR0;
}

FILE* uart_init(int baud){
    //setting the baudrate:
    UBRR0H = (UBRR >> 8);
    UBRR0L = UBRR;
    
    //enable RX0 and TX0:
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    //set frame format.
    //it is currently set to 8 databits, 1 stop bit and none in parity bit
    UCSR0C = (1<<URSEL0) | (3<<UCSZ00);
    return fdevopen(uart_send_byte, uart_receive_byte);
}
