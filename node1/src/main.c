#include <stdint.h>
#define F_CPU 4915200
#include "../lib/IO_board.h"
#include "../lib/joystick.h"
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

    _delay_ms(5);

    // can_message_t message;
    // message.id = 1927;
    // message.data_count = 4;
    // message.data[0] = 'H';
    // message.data[1] = 'e';
    // message.data[2] = 'i';
    // message.data[3] = '!';
// while (1) {

//     can_send(message);
//     printf("Message sent\n");
//     mcp2515_request_send(0x01);
//     char *error;
//     mcp2515_read(0x30, error);
//     printf("%01x\n", *error);

//     _delay_ms(500);
    
// }


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
    main_menu->sub_menus[1]->sub_menus[1] = create_menu(0, "Test2", main_menu->sub_menus[1], 0);
    menu *current_menu = main_menu;
    print_menu(current_menu);

    while(1)
    {
        analog_data = analog_read();
        read_joystick(&joystick_1, analog_data);
        read_touchpad(&touchpad_1);
        read_buttons(&buttons_1);

        send_joystick(&joystick_1);
        // can_message_t msg;
        // can_receive(&msg);
        // printf("%d\n", msg.id);

        current_menu = navigate_menu(current_menu, buttons_1);

        _delay_ms(150);

    }

}
