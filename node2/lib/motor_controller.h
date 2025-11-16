#pragma once
#include "sam.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#define PERIOD_PWM_MOTOR                                                       \
    0b1101001 // setting period to 40µs, calculation can be found at page 1048
              // in the datasheet

void motor_init(void);
void encoder_init(void);
void read_encoder(int32_t *value);
void set_motor_speed(int16_t speed,
                     bool go_left); // sets motor speed dependent on a value
                                    // from 0-100, with 0 being min speed
