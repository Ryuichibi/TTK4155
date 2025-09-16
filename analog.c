#include "analog.h"

#define BASE_ADDR 0x1000


void analog_init(void){
  DDRD |= (1 << PD4);
  TCCR3B |= (1 <<WGM32);
  TCCR3A |= (1 << COM3A0);
  TCCR3B |= (1 << CS30);
  OCR3A  = 0;

}


uint8_t analog_read(uint8_t ch) {
  if (ch) {
    volatile uint8_t adc = (char *) BASE_ADDR;
    *adc = ch; // Prob smth else
    _delay_us(60);
    return *adc; // Get the value

  } else {
    return 0;
  }
}