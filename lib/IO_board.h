#include "analog.h"
#include "spi.h"
#include <stdint.h>
#include <util/delay.h>
#define SSIO_board PB3

typedef struct slider {
    uint8_t x_pos;
    uint8_t size;
} slider;

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
            uint8_t nLeft : 1;
            uint8_t nDown : 1;
            uint8_t nUp : 1;
        };
    };

} buttons;


void read_touchpad(touchpad *touchpad);
void read_slider(slider *slider);
void read_buttons(buttons *buttons);
void digital_write_led(char led, char value);
void analog_write_led(char led, char value);
