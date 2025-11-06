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
#include <avr/interrupt.h>

unsigned char data;
FILE *UART;
joystick joystick_1;
touchpad touchpad_1;
slider slider_1;
buttons buttons_1;
analog_input analog_data;

// Game values
uint8_t gamestate = 0;
uint8_t score = 0;

// Menus
menu *main_menu;
menu *current_menu;

void start_new_game() 
{
    gamestate = 1;
    score = 0;
    oled_reset();
    oled_score(score);
}

void send_inputs(joystick *joystick, uint8_t js_button, touchpad *touchpad)
{
    can_message_t message;
    message.id = JOYSTICK_ID;
    message.data_count = 5;
    message.data[0] = joystick->x_pos;
    message.data[1] = joystick->y_pos;
    message.data[2] = js_button;
    message.data[3] = touchpad->x_pos;
    message.data[4] = touchpad->y_pos;

    printf("Tpad: %d\n", message.data[3]);
    _delay_ms(150);

    can_send(message);
    mcp2515_request_send(0x01);

    char *error;
    mcp2515_read(0x30, error);
    
    if (*error != 0x08) 
    {
        printf("%01x\n", *error);
    } 
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

    _delay_ms(5);

    // Interrupt for CAN receive
    SREG |= (1 << 7); // Enable interrupts
    EMCUCR &= ~(0x01); // Falling edge for INT2
    GICR |= (1 << 5); // Interupt INT2

    // Setup joystick button
    DDRB &= ~(1 << PB0);
    PORTB &= ~(1 << PB0);

    _delay_ms(2000);

    joystick_1.parameters.x_min = 50;
    joystick_1.parameters.y_min = 50;

    // Menu setup
    main_menu = create_menu(3, NULL, NULL, 0, NULL);
    main_menu->sub_menus[0] = create_menu(0, "New Game", main_menu, 0, start_new_game);
    main_menu->sub_menus[1] = create_menu(2, "Scoreboard", main_menu, 0, NULL);
    main_menu->sub_menus[2] = create_menu(0, "Cali Joystick", main_menu, 0, NULL);
    main_menu->sub_menus[1]->sub_menus[0] = create_menu(8, "Highscores", main_menu->sub_menus[1], 0, NULL);
    for (uint8_t i = 0; i < 8; i++) {
        main_menu->sub_menus[1]->sub_menus[0]->sub_menus[i] = create_menu(0, "No score", main_menu->sub_menus[0]->sub_menus[1], 0, NULL);
    }
    main_menu->sub_menus[1]->sub_menus[1] = create_menu(1, "Last Score", main_menu->sub_menus[1], 0, NULL);
    main_menu->sub_menus[1]->sub_menus[1]->sub_menus[0] = create_menu(0, "No score", main_menu->sub_menus[1]->sub_menus[1], 0, NULL);
    current_menu = main_menu;
    print_menu(current_menu);

    while(1)
    {
        switch (gamestate) {
            case 0:
                read_buttons(&buttons_1);
                current_menu = navigate_menu(current_menu, buttons_1);
                _delay_ms(150);
                break;
            case 1:
                analog_data = analog_read();
                read_joystick(&joystick_1, analog_data);
                read_touchpad(&touchpad_1);
                uint8_t js_button = (PINB & (1 << PB0)) >> PB0 ^ 0x01;
                send_inputs(&joystick_1, js_button, &touchpad_1);
                break;
            default:
                printf("Error\n");
                gamestate = 0;
                break;
        }

    }

}

ISR(INT2_vect) {
    uint8_t data;
    mcp2515_read(MCP_CANINTF, &data);

    // Get CAN msg
    can_message_t msg;
    can_receive(&msg);

    // Game end
    if (msg.id == 1) 
    {
        current_menu = main_menu->sub_menus[1]->sub_menus[1]->sub_menus[0]; // Last highscore
        gamestate = 0; // Menu mode
        score = 0; // Reset score
        char *final_score = malloc(sizeof(char) * 4);
        sprintf(final_score, "%d", msg.data[0]);
        main_menu->sub_menus[1]->sub_menus[1]->sub_menus[0]->value = final_score;
        for (uint8_t i = 0; i < main_menu->sub_menus[1]->sub_menus[0]->n_entries; i++) 
        {
            if (main_menu->sub_menus[1]->sub_menus[0]->sub_menus[i]->value == "No score") {
                main_menu->sub_menus[1]->sub_menus[0]->sub_menus[i]->value = final_score;
                break;
            }
        }
        
    }
    // Score
    else if (msg.id == 2)
    {
        score = msg.data[0];
        oled_reset();
        oled_score(score);
    }
    
    // Reset interrupt
    mcp2515_bit_modify(MCP_CANINTF, 0x03, 0x00);


}
