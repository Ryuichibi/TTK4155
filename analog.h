#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

void analog_init(void);
uint8_t analog_read(uint8_t ch);
