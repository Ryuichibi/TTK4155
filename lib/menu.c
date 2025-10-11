#include "menu.h"

menu *create_menu(uint8_t n_entries, char* value, menu* parent, uint8_t selected) 
{

    menu *temp = malloc(sizeof(menu));

    if (n_entries) 
    {
        temp->sub_menus = malloc(sizeof(menu *) * n_entries);
    }

    if (value)
    {
        temp->value = value;
    }

    if (parent)
    {
        temp->parent_menu = parent;
    }

    temp->selected = selected;

    return temp;

}

void print_menu(menu *menu)
{
    for (uint8_t i = 0; i < menu->n_entries; i++) {
        oled_goto_row(0x00 | i);
        oled_goto_column(0x09);
        oled_print(menu->sub_menus[i]->value,
                   strlen(menu->sub_menus[i]->value));
    }
    oled_arrow(menu->selected);
}

void navigate_menu(menu *current_menu, buttons buttons_1) 
{
    // Move up
    if (buttons_1.nDown) 
    {
        oled_arrow_reset(current_menu->selected);
        current_menu->selected = (current_menu->selected == 0)
                                        ? current_menu->n_entries - 1
                                        : current_menu->selected - 1;
        oled_arrow(current_menu->selected);
    } 
    // Move down
    else if (buttons_1.nUp) 
    {
        oled_arrow_reset(current_menu->selected);
        current_menu->selected =
            (current_menu->selected == current_menu->n_entries - 1)
                ? 0
                : current_menu->selected + 1;
        oled_arrow(current_menu->selected);
    }

    // Button press
    if (buttons_1.nButton) 
    {
        printf("%s\n", current_menu->sub_menus[current_menu->selected]->value);
    }

    // Go into submenu
    if (buttons_1.nLeft &&
        current_menu->sub_menus[current_menu->selected]->sub_menus) {
        current_menu = current_menu->sub_menus[current_menu->selected];
        oled_reset();
        print_menu(current_menu);
    } 
    // Go to parent menu
    else if (buttons_1.nRight && current_menu->parent_menu) {
        current_menu = current_menu->parent_menu;
        oled_reset();
        print_menu(current_menu);
    }
}