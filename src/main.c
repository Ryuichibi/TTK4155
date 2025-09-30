#define F_CPU 4915200
#include "../lib/analog.h"
#include "../lib/oled.h"
#include "../lib/spi.h"
#include "../lib/sram.h"
#include "../lib/uart.h"
#include "../lib/menu.h"
#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

unsigned char data;
FILE *UART;
joystick joystick_1;
analog_input analog_data;

void print_main_menu(menu main_menu)
{
  for (uint8_t i = 0; i < main_menu.elements; i++)
    {
      oled_goto_row(0x00 | i);
      oled_goto_column(0x09);
      oled_print(main_menu.data[i], strlen(main_menu.data[i]));
    }
    oled_arrow(main_menu.selected);
}

int main()
{

    UART = uart_init(9600);

    sram_init();
    // sram_test();

    analog_init();
    calib_parameters calibration_values = {0, -1, 0, -1, 0, -1, 0, -1};
    // x_max = left, x_min = right, y_max = down, y_min = up
    joystick_calibrate(&calibration_values);
    printf("y_min: %d, xmin: %d, ymax: %d,x_max %d", calibration_values.y_min,
           calibration_values.x_min, calibration_values.y_max,
           calibration_values.x_max);

    spi_init();
    oled_init();

    menu main_menu;
    main_menu.data = malloc(sizeof(char*) * 8);
    main_menu.size = 8;
    main_menu.elements = 3;
    main_menu.selected = 1;
    main_menu.data[0] = "New game";
    main_menu.data[1] = "Scoreboard";
    main_menu.data[2] = "Cali Joystick";

    print_main_menu(main_menu);

    DDRB &= ~(1 << PB0);
    PORTA &= ~(1 << PA0);


    while (1) {
        joystick_1 = joystick_read(analog_read(), calibration_values);
        
        if (joystick_1.direction_y == 1) 
        {
          oled_arrow_reset(main_menu.selected);
          main_menu.selected = (main_menu.selected == 0) ? main_menu.elements - 1 : main_menu.selected - 1;
        }
        else if (joystick_1.direction_y == 2) 
        {
          oled_arrow_reset(main_menu.selected);
          main_menu.selected = (main_menu.selected == main_menu.elements - 1) ? 0 : main_menu.selected + 1;
        }
        oled_arrow(main_menu.selected);


        if (!(PINB & (1 << PB0))) {
          printf("%s\n", main_menu.data[main_menu.selected]);
        }

        
        _delay_ms(150);
        //spi_send_char(0xaf, PB4);
    }
    return 0;
}
