#include "analog.h"


void analog_init(void){
  DDRD |= (1 << PD4);
  TCCR3B |= (1 <<WGM32);
  TCCR3A |= (1 << COM3A0);
  TCCR3B |= (1 << CS30);
  OCR3A  = 0;

}


