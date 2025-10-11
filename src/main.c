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
#include "../lib/can.h"
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
      printf("in loopback mode\n");
    }

    _delay_ms(5);

    can_message_t message;
    message.id = 1927;
    message.data_count = 4;
    message.data[0] = 'H';
    message.data[1] = 'e';
    message.data[2] = 'i';
    message.data[3] = '!';

    can_send(message);

    _delay_ms(5);
    
    can_message_t message_r;
    can_receive(&message_r);

    printf("ID: %d\n", message_r.id);
    printf("data_count: %d\n", message_r.data_count);
    printf("Data: %s\n", message_r.data);

    // Setup joystick button
    DDRB &= ~(1 << PB0);
    PORTA &= ~(1 << PA0);

    _delay_ms(2000);

    joystick_1.parameters.x_min = 50;
    joystick_1.parameters.y_min = 50;

    // Menu setup
    menu *main_menu = create_menu(3, NULL, NULL, 0);
    main_menu->sub_menus[0] = create_menu(0, "New Game", main_menu, 0);
    main_menu->sub_menus[1] = create_menu(2, "Scoreboard", main_menu, 0);
    main_menu->sub_menus[2] = create_menu(0, "Cali Joystick", main_menu, 0);
    main_menu->sub_menus[1]->sub_menus[0] = create_menu(0, "Test1", main_menu->sub_menus[1], 0);
    main_menu->sub_menus[1]->sub_menus[0] = create_menu(0, "Test2", main_menu->sub_menus[1], 0);
    menu *current_menu = main_menu;
    print_menu(current_menu);

    while (1) {
        analog_data = analog_read();
        read_joystick(&joystick_1, analog_data);
        read_touchpad(&touchpad_1);
        read_buttons(&buttons_1);

        navigate_menu(current_menu, buttons_1);

        _delay_ms(150);
    }
    return 0;
}
