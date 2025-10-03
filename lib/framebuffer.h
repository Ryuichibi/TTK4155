#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H

#include <stdint.h>
#include "font.h"
#include "sram.h"


#define PAGES 8
#define COLUMNS 128

typedef struct {
    uint8_t page;
    uint8_t column;
} Cursor;


// The commands for writing to the framebuffer has been made to ressemble the oled commands

void framebuffer_oled_init();
//void framebuffer_oled_write_data(char data); No use for it yet
void framebuffer_oled_goto_row(uint8_t row);
void framebuffer_oled_goto_column(uint8_t column);
void framebuffer_oled_reset();
void framebuffer_oled_clear_row(uint8_t row);
void framebuffer_oled_print_letter(char data);
void framebuffer_oled_print(const char *str);
void framebuffer_oled_arrow(uint8_t row);
void framebuffer_oled_arrow_reset(uint8_t row);
#endif
