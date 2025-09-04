#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>

#define BIT_TIME_US (1000000 / BAUDRATE)


void uart_send_byte(unsigned char data){
    //Wait until the serial port is ready for sending something new
    while (!(UCSR0A & (1<<UDRE0)))
    ;
    //load new data into sending buffer
    UDR0 = data;
}

    
int uart_init(int baud){
    int UBRR = F_CPU/16/baud -1;
    //setting the baudrate:
    UBRR0H = (UBRR >> 8);
    UBRR0L = UBRR;
    
    //enable RX0 and TX0:
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);

    //set frame format.
    //it is currently set to 8 databits, 1 stop bit and none in parity bit
    UCSR0C = (1<<URSEL0) | (3<<1);
}
