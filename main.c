#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include "uart.h"

int main() {
  uart_init(2400);
  while (1){
    uart_send_byte(0x42);
    //uart_send_byte(0x0A);
    _delay_ms(1000);
  }
	return 0;

}


