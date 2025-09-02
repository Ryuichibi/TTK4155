#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>

#define BAUDRATE 9600
#define BIT_TIME_US (1000000 / BAUDRATE)

void uart_send_byte(uint8_t byte)
{

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


