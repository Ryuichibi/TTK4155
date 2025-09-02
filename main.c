#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>

#include "uart.h"

int main() {

	//DDRA |= (1 << PA0);
	DDRD |= (1 << PD1);

	//PORTD |= (1 << PD1);
	PORTD &= ~(1 << PA1);

//	PORTA |= (1 << PA0);

	while (1) {
		//_delay_ms(3.0);
		//PORTA ^= (1 << PA0);
		//PORTD ^= (1 << PA1);
		uint8_t test = 72;
		//uart_send_byte(test);

	}

	return 0;

}


