#ifndef CAN_H
#define CAN_H

#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include "mcp2515.h"

typedef struct can_message {
    uint16_t id;
    uint8_t data_count;
    uint8_t data[8];
} can_message_t;

void can_init();
void can_send(can_message_t message);
void can_receive(can_message_t *message);

#define JOYSTICK_ID 2

#endif