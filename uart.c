#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>

#define BAUDRATE 9600
#define BIT_TIME_US (1000000 / BAUDRATE)
#define UBRR F_CPU/(16*BAUDRATE) -1

void uart_send_byte(uint8_t byte)
{
    //--------------This code is wrong, there is internal harware for using UART, and we only have to work with that----
    // Start bit (always 0)
    PORTD &= ~(1 << PA1);
    _delay_us(BIT_TIME_US);

    for (uint8_t i = 0; i < 8; i++)
    {
        PORTD &= ((~(1 << PA1)) | ((byte & (1 << i)) >> i));
        PORTD = (PORTD & ~(1 << PA1)) | ((byte >> i & 1) << PA1);
        _delay_us(BIT_TIME_US);
    }

    // TODO: Do we want parity bit?

    // Stop bit (always 1)
    PORTD |= (1 << PA1);
    _delay_us(BIT_TIME_US);

}

//-------------this code is not tested, but is worked out from examples in datasheet--------------
    
int uart_init(){
    //setting the baudrate:
    UBRR0H = (UBRR >> 8);
    UBBR0L = UBBR;
    
    //enable RX0 and TX0:
    UCSR0B = (1<<RXEN) | (1<<TXEN);

    //set frame format.
    //it is currently set to 8 databits, 1 stop bit and none in parity bit
    UCSR0C = (1<<URSEL) | (3<<UCSZ0);
}
