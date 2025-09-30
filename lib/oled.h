#ifndef OLED_H
#define OLED_H

#include <stdint.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "spi.h"
#include "font.h"

void oled_init();
void oled_write_data(char data);
void oled_goto_row(uint8_t line);
void oled_goto_column(uint8_t column);
void oled_clear_row(uint8_t row);
void oled_set_brightness(uint8_t level);

#endif