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
uint16_t score = 0;
uint8_t node2_interrupt = 0;

// Menus
menu *main_menu;
menu *current_menu;

// Define functions
void handle_node2_interrupt();
void send_inputs(joystick *joystick, uint8_t js_button, touchpad *touchpad);
void start_new_game();
void create_main_menu();
void calibrate_joystick();

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

    // Setup joystick button
    DDRB &= ~(1 << PB0);
    PORTB &= ~(1 << PB0);

    _delay_ms(2000);

    joystick_1.parameters.x_min = 50;
    joystick_1.parameters.y_min = 50;

    create_main_menu();

    while(1)
    {
        switch (gamestate) {
            case 0:
                read_buttons(&buttons_1);
                current_menu = navigate_menu(current_menu, buttons_1);
                _delay_ms(150);
                break;
            case 1:
                _delay_ms(20); // needed so can messages arent sent too rapidly
                // Handle the interrupt produced CAN receive
                if (node2_interrupt)
                {
                    handle_node2_interrupt();
                    node2_interrupt = 0;
                }
                uint8_t data;
                mcp2515_read(0x1C, &data);
                printf("TEC: %d\n", data);
                uint8_t data2;
                mcp2515_read(0x1D, &data2);
                printf("REC: %d\n", data2);
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



void create_main_menu() 
{
    // Menu setup
    main_menu = create_menu(3, NULL, NULL, 0, NULL);
    main_menu->sub_menus[0] = create_menu(0, "New Game", main_menu, 0, start_new_game);
    main_menu->sub_menus[1] = create_menu(2, "Scoreboard", main_menu, 0, NULL);
    main_menu->sub_menus[2] = create_menu(0, "Cali Joystick", main_menu, 0, calibrate_joystick);
    main_menu->sub_menus[1]->sub_menus[0] = create_menu(8, "Highscores", main_menu->sub_menus[1], 0, NULL);
    for (uint8_t i = 0; i < 8; i++) {
        main_menu->sub_menus[1]->sub_menus[0]->sub_menus[i] = create_menu(0, "No score", main_menu->sub_menus[1]->sub_menus[0], 0, NULL);
    }
    main_menu->sub_menus[1]->sub_menus[1] = create_menu(1, "Last Score", main_menu->sub_menus[1], 0, NULL);
    main_menu->sub_menus[1]->sub_menus[1]->sub_menus[0] = create_menu(0, "No score", main_menu->sub_menus[1]->sub_menus[1], 0, NULL);
    current_menu = main_menu;
    print_menu(current_menu);
}

void calibrate_joystick()
{
    oled_reset();
    oled_position(3,5);
    oled_print("Spin Joystick",13);
    _delay_ms(500);
    for (uint8_t i = 0; i < 255; i++) {
        analog_data = analog_read();
        read_joystick(&joystick_1, analog_data);
        _delay_ms(20);
    }
    oled_reset();
    oled_position(3,30);
    oled_print("Done", 4);
    _delay_ms(2000);
    oled_reset();
    print_menu(current_menu);
}

void start_new_game() 
{
    gamestate = 1;
    node2_interrupt = 0;
    score = 0;
    can_message_t message;
    message.id = 3;
    message.data_count = 0;
    can_send(message);
    mcp2515_request_send(0x01);
    oled_reset();
    oled_score(score);
}

void send_inputs(joystick *joystick, uint8_t js_button, touchpad *touchpad)
{
    can_message_t message;
    message.id = 4;
    message.data_count = 5;

    if (joystick->x_pos > 45 && joystick->x_pos < 55)
    {
        joystick->x_pos = 50;
    }

    if (joystick->y_pos > 45 && joystick->y_pos < 55)
    {
        joystick->y_pos = 50;
    }

    message.data[0] = joystick->x_pos;
    message.data[1] = joystick->y_pos;
    message.data[2] = js_button;
    message.data[3] = touchpad->x_pos;
    message.data[4] = touchpad->y_pos;

    _delay_ms(10);

    can_send(message);
    mcp2515_request_send(0x01);

    char *error;
    mcp2515_read(0x30, error);
    
    if (*error != 0x08) 
    {
        printf("Error: %01x\n", *error);
    } 
    
}

void handle_node2_interrupt() 
{
    can_message_t msg;
    can_receive(&msg);

    // Game end
    if (msg.id == 1 && score >= 1) 
    {
        gamestate = 0; // Menu mode
        char *final_score = malloc(sizeof(char) * 4);

        final_score[3] = '\0';
        uint8_t hund = score / 100;
        final_score[0] = (char) (hund + '0');
        uint8_t ten = (score - hund * 100) / 10;
        final_score[1] = (char) (ten + '0');
        uint8_t ones = (score - hund * 100 - ten * 10);
        final_score[2] = (char) (ones + '0');

        printf("Int %s\n", final_score);
        score = 0; // Reset score

        main_menu->sub_menus[1]->sub_menus[1]->sub_menus[0]->value = final_score;
        for (uint8_t i = 0; i < main_menu->sub_menus[1]->sub_menus[0]->n_entries; i++) 
        {
            if (main_menu->sub_menus[1]->sub_menus[0]->sub_menus[i]->value == "No score") {
                main_menu->sub_menus[1]->sub_menus[0]->sub_menus[i]->value = final_score;
                break;
            }
        }
        current_menu = main_menu->sub_menus[1]->sub_menus[1]; // Last highscore
        
        oled_reset();
        print_menu(current_menu);
        
    }
    // Update Score
    else if (msg.id == 2)
    {
        score = msg.data[0];
        oled_score(score);
    }

}

// Interrupt vector for CAN receive
ISR(INT2_vect) {
    uint8_t data;
    mcp2515_read(MCP_CANINTF, &data);
    printf("int_d: %d\n", data);

    if (data & 0x01) {
        mcp2515_bit_modify(MCP_CANINTF, 0x01, 0x00);
        node2_interrupt = 1;
    }
    

    // if (data & 0x01) {
    //     printf("\tint\n");
    // }

    //mcp2515_read(MCP_CANINTF, &data);
    //printf("\tint_d: %d\n", data);

}
