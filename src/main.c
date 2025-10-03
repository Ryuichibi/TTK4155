#define F_CPU 4915200

#include <avr/io.h>
#include <stdio.h>
#include <util/delay.h>
#include <string.h>

#include "../lib/analog.h"
#include "../lib/oled.h"
#include "../lib/spi.h"
#include "../lib/sram.h"
#include "../lib/uart.h"
#include "../lib/menu.h"
#include "../lib/framebuffer.h"


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
    calib_parameters calibration_values = {150, 100, 250, 5, 150, 100, 250, 5};
    // x_max = left, x_min = right, y_max = down, y_min = up
    //joystick_calibrate(&calibration_values);
    //printf("y_min: %d, xmin: %d, ymax: %d,x_max %d", calibration_values.y_min,
    //        calibration_values.x_min, calibration_values.y_max,
    //        calibration_values.x_max);

    spi_init();
    oled_init();

    // Setup joystick button
    DDRB &= ~(1 << PB0);    
    PORTA &= ~(1 << PA0);
    
    framebuffer_oled_init();
    menu *main_menu = menu_init();
    menu *current_menu = main_menu;
    print_menu(current_menu);

    while (1) {
      joystick_1 = joystick_read(analog_read(), calibration_values);

      current_menu = menu_handle_input(current_menu, joystick_1);
      oled_update_display();
        
        _delay_ms(200);
        //spi_send_char(0xaf, PB4);
    }
    return 0;
}
