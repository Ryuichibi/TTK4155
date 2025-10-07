#include <stdint.h>
#define F_CPU 4915200
#include "../lib/IO_board.h"
#include "../lib/analog.h"
#include "../lib/menu.h"
#include "../lib/oled.h"
#include "../lib/spi.h"
#include "../lib/sram.h"
#include "../lib/uart.h"
#include "../lib/mcp2515.h"
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <util/delay.h>

unsigned char data;
FILE *UART;
joystick joystick_1;
touchpad touchpad_1;
slider slider_1;
buttons buttons_1;
analog_input analog_data;

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

int main()
{

    UART = uart_init(9600);
    sram_init();
    spi_init();
    analog_init();
    oled_init();
    mcp2515_init();


    mcp2515_bit_modify(0x60, 0b01100000, 0xff);


    mcp2515_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_LOOPBACK);
    _delay_ms(2);
    uint8_t value;
    mcp2515_read(MCP_CANSTAT, &value);
    if ((value & MODE_MASK) == MODE_LOOPBACK) {
      printf("HEadwldalø");
    }

    _delay_ms(5);

    mcp2515_bit_modify(0x2c, 0x01, 0x00);
    mcp2515_write(MCP_TXB0SIDL, 0xff);
    mcp2515_request_send(0x01);
    mcp2515_read(0x2c, &value);
    printf("Stat: %x\r\n", value);
    mcp2515_read(MCP_RXB0SIDL, &value);
    printf("mottar: %x\r\n", value);
    mcp2515_bit_modify(0x2c, 0x01, 0x00);
    mcp2515_write(MCP_TXB0SIDL, 0x44);
    mcp2515_request_send(0x01);
    mcp2515_read(0x2c, &value);
    printf("Stat: %x\r\n", value);
    mcp2515_read(MCP_RXB0SIDL, &value);
    printf("mottar: %x\r\n", value);

    while(1)
    {
    }


    joystick_1.parameters.x_min = 50;
    joystick_1.parameters.y_min = 50;

    menu *main_menu = malloc(sizeof(menu));
    main_menu->sub_menus = malloc(sizeof(menu *) * 3);
    main_menu->n_entries = 3;
    main_menu->selected = 0;
    main_menu->sub_menus[0] = malloc(sizeof(menu));
    main_menu->sub_menus[1] = malloc(sizeof(menu));
    main_menu->sub_menus[2] = malloc(sizeof(menu));
    main_menu->sub_menus[0]->value = "New game";
    main_menu->sub_menus[1]->value = "Scoreboard";
    main_menu->sub_menus[2]->value = "Cali Joystick";
    main_menu->sub_menus[1]->parent_menu = main_menu;
    main_menu->sub_menus[1]->sub_menus = malloc(sizeof(menu *) * 2);
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
        read_buttons(&buttons_1);

        //printf("%d\t", joystick_1.x_pos);
        //printf("%d\n", joystick_1.y_pos);

        // Move along menu entries
        if (buttons_1.nDown) {
            oled_arrow_reset(current_menu->selected);
            current_menu->selected = (current_menu->selected == 0)
                                         ? current_menu->n_entries - 1
                                         : current_menu->selected - 1;
            oled_arrow(current_menu->selected);
        } else if (buttons_1.nUp) {
            oled_arrow_reset(current_menu->selected);
            current_menu->selected =
                (current_menu->selected == current_menu->n_entries - 1)
                    ? 0
                    : current_menu->selected + 1;
            oled_arrow(current_menu->selected);
        }

        if (!(PINB & (1 << PB0)) && current_menu->sub_menus) {
            printf("%s\n",
                   current_menu->sub_menus[current_menu->selected]->value);
        }

        if (joystick_1.direction_x == 1 &&
            current_menu->sub_menus[current_menu->selected]->sub_menus) {
            current_menu = current_menu->sub_menus[current_menu->selected];
            oled_reset();
            print_menu(current_menu);
        } else if (joystick_1.direction_x == 2 && current_menu->parent_menu) {
            current_menu = current_menu->parent_menu;
            oled_reset();
            print_menu(current_menu);
        }

        _delay_ms(150);
    }
    return 0;
}
