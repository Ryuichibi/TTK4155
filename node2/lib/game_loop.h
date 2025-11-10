#pragma once

#include <stdint.h>
#include "sam.h"

#define STANDBY 0
#define GAME 1
#define GAME_STARTUP 2
#define GAME_END 3


int setup_score_counter(void);

int game_loop_statemachine(int8_t *state);


