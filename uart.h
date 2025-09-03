#ifndef UART_H
#define UART_H

#include <avr/io.h>

void uart_send_byte(uint8_t);
int uart_init();
#endif

