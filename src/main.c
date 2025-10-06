#include <stdint.h>
#define F_CPU 4915200
#include "../lib/IO_board.h"
#include "../lib/analog.h"
#include "../lib/oled.h"
#include "../lib/spi.h"
#include "../lib/sram.h"
#include "../lib/uart.h"
#include "../lib/menu.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

unsigned char data;
FILE *UART;
joystick joystick_1;
touchpad touchpad_1;
slider slider_1;
buttons buttons_1;
analog_input analog_data;

void print_menu(menu *menu)
{
  for (uint8_t i = 0; i < menu->n_entries; i++)
  {
    oled_goto_row(0x00 | i);
    oled_goto_column(0x09);
    oled_print(menu->sub_menus[i]->value, strlen(menu->sub_menus[i]->value));
  }
  oled_arrow(menu->selected);
  
}

int main()
{
  
    UART = uart_init(9600);
    sram_init();
    spi_init();
  analog_init();
  joystick_1.parameters.x_min = 50;
  joystick_1.parameters.y_min = 50;

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
    main_menu->sub_menus[1]->sub_menus = malloc(sizeof(menu*) * 2);
    main_menu->sub_menus[1]->n_entries = 2;
    main_menu->sub_menus[1]->selected = 0;
    main_menu->sub_menus[1]->sub_menus[0] = malloc(sizeof(menu));
    main_menu->sub_menus[1]->sub_menus[1] = malloc(sizeof(menu));
    main_menu->sub_menus[1]->sub_menus[0]->value = "Test1";
    main_menu->sub_menus[1]->sub_menus[1]->value = "Test2";


    // Setup joystick button
    DDRB &= ~(1 << PB0);  
    PORTA &= ~(1 << PA0);

    menu *current_menu = main_menu;

    print_menu(current_menu);

    while (1) {
        analog_data = analog_read();
        read_joystick(&joystick_1, analog_data);
        read_touchpad(&touchpad_1);

        printf("%d\t", (uint8_t)touchpad_1.y_pos);
        printf("%d\n", (uint8_t)touchpad_1.x_pos);
    
    
        // Move along menu entries
        if (joystick_1.direction_y == 1) 
        {
          oled_arrow_reset(current_menu->selected);
          current_menu->selected = (current_menu->selected == 0) ? current_menu->n_entries - 1 : current_menu->selected - 1;
          oled_arrow(current_menu->selected);
        }
        else if (joystick_1.direction_y == 2) 
        {
          oled_arrow_reset(current_menu->selected);
          current_menu->selected = (current_menu->selected == current_menu->n_entries - 1) ? 0 : current_menu->selected + 1;
          oled_arrow(current_menu->selected);
        }

        if (!(PINB & (1 << PB0)) && current_menu->sub_menus) {
          printf("%s\n", current_menu->sub_menus[current_menu->selected]->value);
        }

        if (joystick_1.direction_x == 1 && current_menu->sub_menus[current_menu->selected]->sub_menus)
        {
          current_menu = current_menu->sub_menus[current_menu->selected];
          oled_reset();
          print_menu(current_menu);
        }
        else if (joystick_1.direction_x == 2 && current_menu->parent_menu)
        {
          current_menu = current_menu->parent_menu;
          oled_reset();
          print_menu(current_menu);
        }

        
        _delay_ms(150);
        //spi_send_char(0xaf, PB4);

    }
    return 0;
}
