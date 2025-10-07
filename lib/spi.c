#include "spi.h"
#include <avr/io.h>
#include <stdint.h>

void spi_init()
{
    // Set MOSI and SCK and CC as output
    DDRB |= (1 << PB5) | (1 << PB7) | (1 << PB4);

    // Set MISO as input
    DDRB &= ~(1 << PB6);

    // Set up SS signals
    DDRB |= (1 << PB4) | (1 << PB3) | (1 << PB2);
    PORTB |= (1 << PB4) | (1 << PB3) | (1 << PB2);

    // Enable SPI, Set as Master, SCK = f_osc / 16
    SPCR = (1 << SPE) | (1 << MSTR) | (1 << SPR0);

    SPSR &= ~(1 << SPI2X);
}

void spi_send_receive(char data, char *out)
{

    // Put data into shift register
    SPDR = data;

    // Wait for data transmit to complete
    while (!(SPSR & (1 << SPIF)))
        ;
    *out = SPDR;
}

// do we actually need the following function?
void spi_send_string(char *data, uint8_t size, uint8_t ss_port)
{
    PORTB &= ~(1 << ss_port);
    char *flush_buffer;

    for (uint8_t i = 0; i < size; i++) {
        spi_send_receive(data[i], flush_buffer);
    }

    PORTB |= (1 << ss_port);
}

void spi_send_char(char data)
{
    char *flush_buffer;
    spi_send_receive(data, flush_buffer);
}

void spi_receive_char(char *data)
{
    spi_send_receive(0x00, data);
}

void spi_open_com(uint8_t ss_port){
    PORTB &= ~(1 << ss_port);
}

void spi_close_com(uint8_t ss_port){
    PORTB |= (1 << ss_port);
}


