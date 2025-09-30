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

    while (1) {
        read_touchpad(&touchpad_1);
        printf("%d\t", touchpad_1.x_pos);
        printf("%d\t", touchpad_1.y_pos);
        printf("%d\n", touchpad_1.size);
        _delay_ms(500);
    }
    return 0;
}
