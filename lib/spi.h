#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#define F_CPU 4915200
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>

void spi_init();
void spi_send_receive(char data, char *out);
void spi_send_string(char *data, uint8_t size, uint8_t ss_port);
void spi_send_char(char data, uint8_t ss_port);
void spi_receive_char(char *data, uint8_t ss_port);
void spi_open_com(uint8_t ss_port);
void spi_close_com(uint8_t ss_port);

#endif
