#ifndef UART_H
#define UART_H

#include <avr/io.h>
#include <stdio.h>

int uart_send_byte(char data,FILE* file);
int uart_receive_byte (FILE* file);
FILE* uart_init(int baud);

#endif

