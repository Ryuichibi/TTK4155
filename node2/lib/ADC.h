#pragma once
#include "sam.h"

#define PMC_PCER1_ADC PMC_PCER1_PID37
#define ADC_MR_CLEAR 0
#define ADC_CWR_THRESHOLD 1500

void ADC_init (void);
void adcStartConversion (void);
