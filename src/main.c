#include <stdint.h>
#define F_CPU 4915200
#include "../lib/IO_board.h"
#include "../lib/analog.h"
#include "../lib/oled.h"
#include "../lib/spi.h"
#include "../lib/sram.h"
#include "../lib/uart.h"
#include <avr/io.h>
#include <stdio.h>
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
  joystick_1.parameters.x_min = 50;
  joystick_1.parameters.y_min = 50;


    while (1) {
        analog_data = analog_read();
        read_joystick(&joystick_1, analog_data);
        read_touchpad(&touchpad_1);

        printf("%d\t", (uint8_t)touchpad_1.y_pos);
        printf("%d\n", (uint8_t)touchpad_1.x_pos);
        _delay_ms(500);
    }
    return 0;
}
