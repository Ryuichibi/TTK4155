#include "controller.h"
#include <stdint.h>

__attribute__((constructor)) void init_controller(void)
{
    PMC->PMC_PCER0 |= (1 << ID_TC0);
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE | TC_CMR_TCCLKS_TIMER_CLOCK1;
    TC0->TC_CHANNEL[0].TC_RC = 84000;
    TC0->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS;
    NVIC_EnableIRQ((IRQn_Type)TC0_IRQn);
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;

}

