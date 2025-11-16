#include "ADC.h"
#include "sam3x8e.h"


void ADC_init(){
    PMC->PMC_PCER1 |= PMC_PCER1_ADC;
    ADC->ADC_MR = ADC_MR_CLEAR;
    ADC->ADC_CHER = ADC_CHER_CH0;
    ADC->ADC_IER = ADC_IER_COMPE;
    ADC->ADC_EMR = ADC_EMR_CMPMODE_LOW;
    ADC->ADC_CWR = ADC_CWR_LOWTHRES(ADC_CWR_THRESHOLD);


    NVIC_EnableIRQ(ADC_IRQn);
}

void adcStartConversion (void){
    ADC->ADC_CR = 2;
}
