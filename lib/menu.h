#ifndef MENU_H
#define MENU_H

#include <avr/io.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "oled.h"
#include "IO_board.h"

typedef struct menu menu;

struct menu {
    char *value;
    menu **sub_menus;
    uint8_t n_entries;
    uint8_t selected;
    menu *parent_menu;
};

menu * create_menu(uint8_t n_entries, char* value, menu* parent, uint8_t selected);
void print_menu(menu *menu);
void navigate_menu(menu *current_menu, buttons buttons_1);


#endif