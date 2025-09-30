#define F_CPU 4915200
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
analog_input analog_data;

int main()
{

    UART = uart_init(9600);

    sram_init();
    // sram_test();

    analog_init();
    calib_parameters calibration_values = {0, -1, 0, -1, 0, -1, 0, -1};
    // x_max = left, x_min = right, y_max = down, y_min = up
    // joystick_calibrate(&calibration_values);
    printf("y_min: %d, xmin: %d, ymax: %d,x_max %d", calibration_values.y_min,
           calibration_values.x_min, calibration_values.y_max,
           calibration_values.x_max);

    spi_init();
    oled_init();

    spi_init();
    oled_init();
    oled_goto_row(0x02);
    oled_goto_column(0x40);

    while (1) {
        oled_write_data(0xff);
        spi_send_char(0xaf, PB4);
    }
    return 0;
}
