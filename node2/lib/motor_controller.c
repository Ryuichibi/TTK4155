#include "motor_controller.h"

// Timer/Counter 2
void encoder_init(void){
    // Set up pins
    PIOC->PIO_PDR |= PIO_PDR_P25;
    PIOC->PIO_ABSR |= PIO_ABSR_P25;
    PIOC->PIO_PDR |= PIO_PDR_P26;
    PIOC->PIO_ABSR |= PIO_ABSR_P26;


    REG_PMC_PCER0 |= PMC_PCER0_PID27 | PMC_PCER0_PID28 | PMC_PCER0_PID29 | PMC_PCER0_PID30 | PMC_PCER0_PID31;
    REG_PMC_PCER1 |= PMC_PCER1_PID32 | PMC_PCER1_PID33 | PMC_PCER1_PID34 | PMC_PCER1_PID35;
    
    // XC0 and capture mode
    TC2->TC_CHANNEL[0].TC_CMR = TC_CMR_TCCLKS_XC0;
    // activate quadrature encoder, position measure mode
    TC2->TC_BMR = TC_BMR_POSEN | TC_BMR_QDEN | TC_BMR_EDGPHA | TC_BMR_INVA;
    // enable clock, reset counter
    TC2->TC_CHANNEL[0].TC_CCR = TC_CCR_CLKEN | TC_CCR_SWTRG;
}

void motor_init(void){
    PMC->PMC_PCER1 |= PMC_PCER1_PID36;
    PWM->PWM_CLK |= PWM_CLK_DIVA(1) | PWM_CLK_PREA(5);

    PIOB->PIO_PDR |= PIO_PDR_P12;
    PIOB->PIO_MDDR |= PIO_MDDR_P12;
    PIOB->PIO_ABSR |= PIO_ABSR_P12;

    PWM->PWM_CH_NUM[0].PWM_CMR = PWM_CMR_CPRE_CLKA;
    PWM->PWM_CH_NUM[0].PWM_CPRD = PERIOD_PWM_MOTOR; //setting period according to define in header
    PWM->PWM_CH_NUM[0].PWM_CDTY = PERIOD_PWM_MOTOR; //setting duty cycle to 100% for startup
    PWM->PWM_ENA |= PWM_ENA_CHID0;



    PIOC->PIO_OER |= PIO_OER_P23;
    PIOC->PIO_CODR |= PIO_CODR_P23;
    PIOC->PIO_PER |= PIO_PSR_P23;
}

void read_encoder(int32_t *value){
    *value = TC2->TC_CHANNEL[0].TC_CV;
}

void set_motor_speed(int16_t speed, bool go_left){
    PWM->PWM_CH_NUM[0].PWM_CDTYUPD = (PERIOD_PWM_MOTOR * (-1*(speed-100))) / 100;
    if (go_left){
        PIOC->PIO_CODR |= PIO_CODR_P23;
    } else {
        PIOC->PIO_SODR |= PIO_SODR_P23;
    }
}
