#include "stepper.h"
#include "component/component_pwm.h"


void stepper_init(){
    //enable output pin for the stepper
    PIOB->PIO_PDR |= PIO_PDR_P13;
    PIOB->PIO_MDDR |= PIO_MDDR_P13;
    PIOB->PIO_ABSR |= PIO_ABSR_P13;

    //enablae Peripheral clock for PWM
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    
    //setup parameters for PWM for the stepper
    PWM->PWM_CLK = PWM_CLK_DIVA(1) | PWM_CLK_PREA(5);
    PWM->PWM_CH_NUM[1].PWM_CMR = PWM_CMR_CPRE_CLKA;
    PWM->PWM_CH_NUM[1].PWM_CPRD = STEPPER_PERIOD;
    PWM->PWM_CH_NUM[1].PWM_CDTY = CDTY_L;

    PWM->PWM_ENA |= 2;


}
