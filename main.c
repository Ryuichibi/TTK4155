#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include "sram.h"
#include "analog.h"


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
  joystick_calibrate(&calibration_values);
  printf("y_min: %d, xmin: %d, ymax: %d,x_max %d",calibration_values.y_min, calibration_values.x_min, calibration_values.y_max, calibration_values.x_max);
  //  sram_write(6, 15);
  //printf("%d", sram_read(15));
  while (1) {
    analog_data = analog_read();
    joystick_1 = joystick_read(analog_data, calibration_values);
    printf("x direction: %d \n y direction: %d", joystick_1.x_pos, joystick_1.y_pos);
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


