#define F_CPU 4915200
#include "../lib/analog.h"
#include "../lib/oled.h"
#include "../lib/spi.h"
#include "../lib/sram.h"
#include "../lib/uart.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

unsigned char data;
FILE *UART;
joystick joystick_1;
analog_input analog_data;

struct menu {
  char **data;
  uint8_t elements;
  uint8_t selected;
};

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

    struct menu main_menu;
    main_menu.data = malloc(sizeof(char*) * 8);
    main_menu.elements = 8;
    main_menu.selected = 0;
    main_menu.data[0] = "New game";
    
    //char main_menu[8][16] = {"New game", "Scoreboard", "Reset", "Calib Joystick", "Difficulty", "", "Debug", ""};
    for (uint8_t i = 0; i < main_menu.elements; i++)
    {
      oled_goto_row(0x00 | i);
      oled_goto_column(0x08);
      oled_print(main_menu.data[i], strlen(main_menu.data[i]));
    }

    // oled_goto_row(0x02);
    // oled_goto_column(0x00);
    // oled_print("Reset high score", 16);
    // oled_goto_row(0x03);
    // oled_goto_column(0x00);
    // oled_print("   Test2", 7);
    oled_goto_row(0x00);
    oled_goto_column(0x00);

    while (1) {
        //oled_write_data(0xff);
        spi_send_char(0xaf, PB4);
    }
    return 0;
}
