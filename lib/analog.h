#ifndef ANALOG_H_
#define ANALOG_H_
#include <stdint.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct analog_input{
  uint8_t analog_ch0;
  uint8_t analog_ch1;
  uint8_t analog_ch2;
  uint8_t analog_ch3;
}analog_input;


void analog_init(void);
analog_input analog_read();



#endif
