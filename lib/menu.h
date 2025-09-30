#ifndef MENU_H
#define MENU_H

typedef struct menu menu;

struct menu {
    char *value;
    menu **sub_menus;
    uint8_t n_entries;
    uint8_t selected;
    menu *parent_menu;
};


#endif