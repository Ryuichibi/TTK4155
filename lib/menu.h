#ifndef MENU_H
#define MENU_H

typedef struct menu {
    char **data;
    uint8_t size;
    uint8_t elements;
    uint8_t selected;
} menu;


#endif