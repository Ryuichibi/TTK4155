#include "menu.h"

menu* menu_init()
{
    menu *main_menu = malloc(sizeof(menu));
    main_menu->sub_menus = malloc(sizeof(menu*) * 3);
    main_menu->n_entries = 3;
    main_menu->selected = 0;
    main_menu->sub_menus[0] = malloc(sizeof(menu));
    main_menu->sub_menus[1] = malloc(sizeof(menu));
    main_menu->sub_menus[2] = malloc(sizeof(menu));
    main_menu->sub_menus[0]->value = "New game";
    main_menu->sub_menus[1]->value = "Scoreboard";
    main_menu->sub_menus[2]->value = "Cali Joystick";
    main_menu->sub_menus[1]->parent_menu = main_menu;
    main_menu->sub_menus[1]->sub_menus = malloc(sizeof(menu*) * 3);
    main_menu->sub_menus[1]->n_entries = 3;
    main_menu->sub_menus[1]->selected = 0;
    main_menu->sub_menus[1]->sub_menus[0] = malloc(sizeof(menu));
    main_menu->sub_menus[1]->sub_menus[1] = malloc(sizeof(menu));
    main_menu->sub_menus[1]->sub_menus[2] = malloc(sizeof(menu)); //Kato
    main_menu->sub_menus[1]->sub_menus[0]->value = "Test1";
    main_menu->sub_menus[1]->sub_menus[1]->value = "Test2";
    main_menu->sub_menus[1]->sub_menus[2]->value = "Test10"; //Kato
    return main_menu;
}

void print_menu(menu *menu)
{
  for (uint8_t i = 0; i < menu->n_entries; i++)
  {
    framebuffer_goto_line(i);
    framebuffer_goto_column(0x09);
    framebuffer_print_string(menu->sub_menus[i]->value);
    
    
    //oled_print(menu->sub_menus[i]->value, strlen(menu->sub_menus[i]->value));
  }
    framebuffer_arrow(menu->selected);
  
}

menu* menu_handle_input(menu *current_menu, joystick joystick_1)
{       
        // Move along menu entries
        if (joystick_1.direction_y == 1) 
        {
          framebuffer_arrow_reset(current_menu->selected);
          current_menu->selected = (current_menu->selected == 0) ? current_menu->n_entries - 1 : current_menu->selected - 1;
          framebuffer_arrow(current_menu->selected);
        }
        else if (joystick_1.direction_y == 2) 
        {
          framebuffer_arrow_reset(current_menu->selected);
          current_menu->selected = (current_menu->selected == current_menu->n_entries - 1) ? 0 : current_menu->selected + 1;
          framebuffer_arrow(current_menu->selected);
        }

        if (!(PINB & (1 << PB0)) && current_menu->sub_menus) {
          printf("%s\n", current_menu->sub_menus[current_menu->selected]->value);
        }

        if (joystick_1.direction_x == 1 && current_menu->sub_menus[current_menu->selected]->sub_menus)
        {
          //current_menu = current_menu->sub_menus[current_menu->selected];
          //oled_reset();
          //print_menu(current_menu);
        }
        else if (joystick_1.direction_x == 2 && current_menu->parent_menu)
        {
          //current_menu = current_menu->parent_menu;
          //oled_reset();
          //print_menu(current_menu);
        }

    return current_menu;
}

