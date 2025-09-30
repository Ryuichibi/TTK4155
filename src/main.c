#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "../lib/uart.h"
#include "../lib/sram.h"
#include "../lib/analog.h"
#include "../lib/spi.h"
#include "../lib/oled.h"


unsigned char data;
FILE* UART;
joystick joystick_1;
analog_input analog_data;


int main() {

  UART = uart_init(9600);

  
  sram_init();
  //sram_test();

  analog_init();
  calib_parameters calibration_values = {0, -1, 0, -1, 0, -1, 0, -1};
  //x_max = left, x_min = right, y_max = down, y_min = up
  //joystick_calibrate(&calibration_values);
  printf("y_min: %d, xmin: %d, ymax: %d,x_max %d",calibration_values.y_min, calibration_values.x_min, calibration_values.y_max, calibration_values.x_max);

  spi_init();
  oled_init();
  oled_goto_row(0x02);
  oled_goto_column(0x40);

  while (1) {
    //analog_data = analog_read();
    //joystick_1 = joystick_read(analog_data, calibration_values);

    // oled_write_data(0b01000001);
    // oled_write_data(0b01111111);
    // oled_write_data(0b01111111);
    // oled_write_data(0b01001001);
    // oled_write_data(0b00011101);
    // oled_write_data(0b00000001);
    // oled_write_data(0b00000011);
    // oled_write_data(0b00000000);

    oled_write_data(0xff);
    spi_send_char(0xaf, PB4);

    // spi_send_char(0xa5, PB4);
    // _delay_ms(2000);
    // spi_send_char(0xa4, PB4);
    // _delay_ms(2000);




    //printf("%d %d\n", analog_data.analog_ch2, analog_data.analog_ch3);
    //if (joystick_1.direction_x == RIGHT) printf("Right\n");
    //if (joystick_1.direction_x == LEFT) printf("Left\n");
    //if (joystick_1.direction_y == FORWARD) printf("Forward\n");
    //if (joystick_1.direction_y == BACKWARD) printf("Backward\n");
    //if (joystick_1.direction_x == NEUTRALX) printf("Neutral_x\n");
    //if (joystick_1.direction_y == NEUTRALY) printf("Neutral_y\n");
    //printf("x direction: %d \n y direction: %d", joystick_1.x_pos, joystick_1.y_pos);
  //  printf("y:%d, x:%d\n", analog_data.analog_ch0,analog_data.analog_ch1);
  }

  //data = (unsigned char) uart_receive_byte(UART);
  //printf("Yes");
  //sram_write(10, 10);
  

  // DDRA |= (1<< PA0);
  // while (1){
  //   printf("Hello, %d", 12);
  //   _delay_ms(1000);
  //   data = (unsigned char) uart_receive_byte(UART);
  //   if (data == 'T') PORTA ^= (1<<PA0);
  // }


	return 0;

}


