#pragma once
#include "sam.h"

#define CDTY_L 0b1100001011111000u // This is 1ms
#define CDTY_H 0b1011100001101100u // This is 2ms 
#define CDTY_STEP_DIFF 0b11011u
#define STEPPER_PERIOD 0b1100110100010100

void stepper_init(void);
