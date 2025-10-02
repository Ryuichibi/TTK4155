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

void framebuffer_init();
void framebuffer_goto_line(uint8_t line);
void framebuffer_goto_column(uint8_t column);
void framebuffer_print_letter(char data);
void framebuffer_print_string(const char *str);

#endif
