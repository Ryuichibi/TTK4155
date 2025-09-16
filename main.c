#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "uart.h"
#include "sram.h"
#include "analog.h"


unsigned char data;
FILE* UART;
struct joystick joystick_1;
analog_input analog_data;

int main() {

  UART = uart_init(9600);

  
  sram_init();
  //sram_test();

  analog_init();
  //  sram_write(6, 15);
  //printf("%d", sram_read(15));
  while (1) {
    analog_data = analog_read();
    printf("x:%d, y: %d\n", analog_data.analog_ch3, analog_data.analog_ch4);
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


