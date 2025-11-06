#pragma once
#include "motor_controller.h"
#include "sam.h"
#include <stdint.h>

#define CONTROLLER_PERIOD 0.002 // period of the controller in seconds
#define K_P 0.02
#define K_I 2


void init_controller(void);