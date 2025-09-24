#include "spi.h"

void spi_init()
{
    // Set MOSI and SCK and CC as output
    DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);

    // Set MISO as input
    DDRB &= ~(1<<PB6);

    // Set up SS signals
    DDRB |= (1<<PB4) | (1<<PB3);
    PORTB |= (1<<PB4) | (1<<PB3);

    // Enable SPI, Set as Master, SCK = f_osc / 16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    SPSR &= ~(1<<SPI2X);

}

void spi_send(char data)
{

    // Put data into shift register
    SPDR = data;

    // Wait for data transmit to complete
    while (!(SPSR & (1<<SPIF)));

}

void spi_send_string(char *data, uint8_t size, uint8_t ss_port)
{
    PORTB &= ~(1 << ss_port);

    for (uint8_t i = 0; i < size; i++)
    {
        spi_send(data[i]);
    }

    PORTB |= (1 << ss_port);

}

void spi_send_char(char data, uint8_t ss_port)
{
    PORTB &= ~(1 << ss_port);
    spi_send(data);
    PORTB |= (1 << ss_port);

}