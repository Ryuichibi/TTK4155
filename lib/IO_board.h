#include "analog.h"
#include "spi.h"
#include <stdint.h>
#include <util/delay.h>
#define SSIO_board PB3

enum directionX { NEUTRALX, RIGHT, LEFT };

enum directionY { NEUTRALY, FORWARD, BACKWARD };

typedef struct calib_parameters {
    uint8_t x_neutral_max;
    uint8_t x_neutral_min;
    uint8_t x_max;
    uint8_t x_min;
    uint8_t y_neutral_max;
    uint8_t y_neutral_min;
    uint8_t y_max;
    uint8_t y_min;
} calib_parameters;

typedef struct slider {
    char x_pos_raw;
    uint8_t x_pos;
    uint8_t size;
} slider;

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

typedef struct buttons {
    union {
        char right;
        struct {
            uint8_t R1 : 1;
            uint8_t R2 : 1;
            uint8_t R3 : 1;
            uint8_t R4 : 1;
            uint8_t R5 : 1;
            uint8_t R6 : 1;
        };
    };

    union {
        char left;
        struct {
            uint8_t L1 : 1;
            uint8_t L2 : 1;
            uint8_t L3 : 1;
            uint8_t L4 : 1;
            uint8_t L5 : 1;
            uint8_t L6 : 1;
            uint8_t L7 : 1;
        };
    };
    union {
        char nav;
        struct {
            uint8_t nButton : 1;
            uint8_t nRight : 1;
            uint8_t nDown : 1;
            uint8_t nLeft : 1;
            uint8_t nUp : 1;
        };
    };

} buttons;

typedef struct touchpad {
    uint8_t x_pos_raw;
    uint8_t y_pos_raw;
    
    uint8_t x_pos;
    uint8_t y_pos;
    uint8_t size;
} touchpad;

void read_slider(slider *slider);
void read_buttons(buttons *buttons);
void digital_write_led(char led, char value);
void analog_write_led(char led, char value);
void joystick_calibrate(joystick *joystick);
void read_touchpad(touchpad *touchpad);
void read_joystick(joystick *joystick, analog_input analog_in);
