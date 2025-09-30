#include "analog.h"
#include "spi.h"
#include <stdint.h>
#include <util/delay.h>
#define SSIO_board PB3 //unsure if this is the correct pin

typedef struct slider {
    uint8_t x_pos;
    uint8_t size;
} slider;

typedef struct buttons {
    char right;
    char left;
    char nav;
} buttons;

typedef struct ledOnOff {
    bool led0;
    bool led1;
    bool led2;
    bool led3;
    bool led4;
    bool led5;
} led;


void read_touchpad(touchpad *touchpad);
void read_slider(slider *slider);
void read_buttons(buttons *buttons);
