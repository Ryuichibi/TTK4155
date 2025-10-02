#include "framebuffer.h"

Cursor cursor = {0,0};

void framebuffer_init(){
    for (int page = 0; page < PAGES; page++){
        for (int column = 0; column < COLUMNS; column++){
            sram_write(0x00, page * COLUMNS + column);
        }
    }
}

void framebuffer_goto_line(uint8_t line){
    cursor.page = line;
}

void framebuffer_goto_column(uint8_t column){
    cursor.column = column;
}

void framebuffer_print_letter(char data)
{
    for (uint8_t i = 0; i < 8; i++) {
        char encoded_data = pgm_read_byte(&font8[(uint8_t)data - 0x20][i]); // probably needs an offset of 20 for the encoding
                           // to work properly, but it should print something
                           // that looks like an alphanumerical character
        sram_write(encoded_data, cursor.page * COLUMNS + cursor.column + i);
    }
    cursor.column += 8;
    if (cursor.column >= COLUMNS) {
        cursor.column = 0;
        cursor.page++;
        if (cursor.page >= PAGES) {
            cursor.page = 0;
        }
    }
 }

void framebuffer_print_string(const char *str)
{
    while (*str) {                  // loop until null terminator
        framebuffer_print_letter(*str);  // print current letter
        str++;                        // move to next character
    }
}


void framebuffer_arrow(uint8_t row)
{
    framebuffer_goto_line(row);
    framebuffer_goto_column(2);
    sram_write(0b00011000, cursor.page * COLUMNS + cursor.column++); 
    sram_write(0b01111110, cursor.page * COLUMNS + cursor.column++);
    sram_write(0b00111100, cursor.page * COLUMNS + cursor.column++);
    sram_write(0b00011000, cursor.page * COLUMNS + cursor.column++);
}

void framebuffer_arrow_reset(uint8_t row)
{
    framebuffer_goto_line(row);
    framebuffer_goto_column(2);
    sram_write(0x00, cursor.page * COLUMNS + cursor.column++);
    sram_write(0x00, cursor.page * COLUMNS + cursor.column++);
    sram_write(0x00, cursor.page * COLUMNS + cursor.column++);
    sram_write(0x00, cursor.page * COLUMNS + cursor.column++);
}