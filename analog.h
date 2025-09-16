#include <stdint.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>

typedef struct analog_input{
  uint8_t analog_ch1;
  uint8_t analog_ch2;
  uint8_t analog_ch3;
  uint8_t analog_ch4;
}analog_input;


void analog_init(void);
analog_input analog_read();

void joystick_read();
typedef struct joystick {
  uint8_t x_pos;
  uint8_t y_pos;
  //bool button;
}joystick;

//enum joystick_direction{
////  LEFT,
//  RIGHT,
//  UP,
//  DOWN,
//  NEUTRAL
//}
