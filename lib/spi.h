#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

void spi_init();
void spi_send(char data);
void spi_send_string(char *data, uint8_t size, uint8_t ss_port);
void spi_send_char(char data, uint8_t ss_port);

#endif