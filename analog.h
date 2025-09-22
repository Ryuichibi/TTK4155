#include <stdint.h>
#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct analog_input{
  uint8_t analog_ch0;
  uint8_t analog_ch1;
  uint8_t analog_ch2;
  uint8_t analog_ch3;
}analog_input;


void analog_init(void);
analog_input analog_read();

enum directionX{
  NEUTRAL,
  RIGHT,
  LEFT
};

enum directionY{
  NEUTRAL,
  FORWARD,
  BACKWARD
};

typedef struct joystick {
  int8_t x_pos;
  int8_t y_pos;
  
  bool button;
  enum directionY direction_y;
  enum directionX direction_x;

}joystick;

joystick joystick_read();//TODO takes in the wrong inputs compared to the function in .c

typedef struct touchpad {
  uint8_t x_pos;
  uint8_t y_pos;
}touchpad;

touchpad touchpad_read();//TODO takes in the wrong inputs compared to the function in .c
