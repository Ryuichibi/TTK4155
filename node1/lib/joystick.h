#ifndef JOYSTICK_H
#define JOYSTICK_H

#include "IO_board.h"
#include "can.h"

typedef struct joystick {
    uint8_t x_pos_raw;
    uint8_t y_pos_raw;
    char button;

    int8_t x_pos;
    int8_t y_pos;

    enum directionY direction_y;
    enum directionX direction_x;
    calib_parameters parameters;

} joystick;

void read_joystick(joystick *joystick, analog_input analog_in);
void send_joystick(joystick *joystick);
void joystick_calibrate(joystick *joystick);

#endif