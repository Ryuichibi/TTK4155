#include "controller.h"



int init_controller(){
    PMC->PMC_PCER0 |= ID_TC0;
    TC0->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVSEL_UP_RC | TC_CMR_WAVE;
    TC0->TC_CHANNEL[0].TC_RC = 7000000;
    TC0->TC_CHANNEL[0].TC_IER |= TC_IER_CPCS;
    NVIC_EnableIRQ((IRQn_Type)TC0_IRQn);
    TC0->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG; 
}

void TC0_Handler(){
    //insert controller code here    
    TC0->TC_CHANNEL[0].TC_SR;
    NVIC_ClearPendingIRQ(ID_TC0);
} 



