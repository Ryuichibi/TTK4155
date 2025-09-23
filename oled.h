#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "spi.h"

void oled_init();
void oled_write_data(char data);

#endif