#include "game_loop.h"
#include <stdint.h>
#include <stdio.h>



__attribute__((constructor)) void setup_score_counter(void){
    PMC->PMC_PCER0 |= ID_TC1;
    TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE;
    TC1->TC_CHANNEL[0].TC_RC = 7000000; //this needs to be changed to a value corresponding to 1s
    TC1->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS;
    NVIC_EnableIRQ((IRQn_Type)TC1_IRQn);
    TC1->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;


}

int game_loop_statemachine(int8_t *state){
    uint8_t i;

    switch (*state) {
     case STANDBY:
        if (i = 255){
            printf("Machine is in standby");
            i=0;
        }else {
            i += 1;
        }
        break;

    case GAME_STARTUP:
        TC0->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS; // enable timer interupt for the PI-controller
        TC1->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS; // enable timer interupt for the score counter
        *state = GAME;
            printf("Starting game");
        break;

    case GAME:
        //read_adc();
        //read_can_msg();
        // send canmsg with score every second, maybe as a part of the interrupt handler?
        break;

    case GAME_END:
        TC0->TC_CHANNEL[0].TC_IDR |= TC_IDR_CPCS; // disable the timer for the PI- controller
        TC1->TC_CHANNEL[0].TC_IDR |= TC_IDR_CPCS; // disabke timer interrup for the score counter
        *state = STANDBY;
        //send canmsg with score
        printf("Game Ended");
        break;

    default:
        *state = STANDBY;
        break;


    }


}
