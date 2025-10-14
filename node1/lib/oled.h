#ifndef OLED_H
#define OLED_H

#define F_CPU 4915200
//#include "../font.h"
#include "spi.h"
#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include "font.h"

void oled_init();
void oled_write_data(char data);
void oled_write_command(char command);
void oled_goto_row(uint8_t row);
void oled_goto_column(uint8_t column);
void oled_reset();
void oled_clear_row(uint8_t row);
void oled_print_letter(char data);
void oled_print(char *data, uint8_t size);
void oled_arrow(uint8_t row);
void oled_arrow_reset(uint8_t row);


//--------------command defines that are useful------------------

//-----Commands for setup
#define DISPLAY_OFF 0xAE
#define SET_SEGMENT_REMAP 0xA0
#define SET_COM_PINS 0xDA
#define ALTERNATIVE_COM_CONFIG 0x12
#define SET_COM_OUTPUT_DIRECTION 0xC0
#define SET_MULTIPLEX_RATIO 0xA8
#define MULTIPLEX_RATIO 0x3F
#define SET_DISPLAY_CLOCK 0xD5
#define DISPLAY_CLOCK 0x80
#define SET_CONTRAST_CONTROL 0x81
#define CONTRAST 0xF0
#define SET_PRECHARGE_PERIOD 0xD9
#define PRECHARGE_PERIOD 0x21
#define SET_MEMORY_ADDRESSING_MODE 0x20
#define MEMORY_MODE_PAGE 0x02
#define SET_VCOMH_LEVEL 0xDB
#define VCOMH_LEVEL 0x30
#define DISPLAY_RESUME_RAM_CONTENT 0xA4
#define SET_DISPLAY_NOT_INVERTED 0xA6
#define SET_DISPLAY_INVERTED 0xa7
#define DISPLAY_ON_NORMAL_MODE 0xAF

//-----Commands for control
#define SET_PAGE 0xB0
#define SET_LOWER_COLUMN_START_ADDRESS 0x00
#define SET_HIGHER_COLUMN_START_ADDRESS 0x10

#endif
