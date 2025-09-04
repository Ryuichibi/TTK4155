#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"


unsigned char data;
FILE* UART;

int main() {
  UART = uart_init(9600);
  DDRA |= (1<< PA0);
  while (1){
    printf("hei");
    _delay_ms(1000);
    data = (unsigned char) uart_receive_byte(UART);
    if (data == 'T') PORTA ^= (1<<PA0);
  }
	return 0;

}


