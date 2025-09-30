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
    // sram_test();

    // x_max = left, x_min = right, y_max = down, y_min = up
    // joystick_calibrate(&calibration_values);

    spi_init();
    oled_init();

    oled_goto_row(0x02);
    oled_goto_column(0x40);

    char *flush;
    while (1) {
    }
    return 0;
}
