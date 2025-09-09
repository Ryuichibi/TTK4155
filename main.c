#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include "sram.h"


unsigned char data;
FILE* UART;

int main() {

  UART = uart_init(9600);

  sram_init();
  sram_test();

  // DDRA |= (1<< PA0);
  // while (1){
  //   printf("Hello, %d", 12);
  //   _delay_ms(1000);
  //   data = (unsigned char) uart_receive_byte(UART);
  //   if (data == 'T') PORTA ^= (1<<PA0);
  // }


	return 0;

}


