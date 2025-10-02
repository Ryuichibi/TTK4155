#ifndef MENU_H
#define MENU_H
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "framebuffer.h"
#include "analog.h"

typedef struct menu menu;

struct menu {
    char *value;
    menu **sub_menus;
    uint8_t n_entries;
    uint8_t selected;
    menu *parent_menu;
};

menu* menu_init();
void print_menu(menu *menu);
menu* menu_handle_input(menu *current_menu, joystick joystick_1);

#endif