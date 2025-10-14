#include "analog.h"

#define BASE_ADDR 0x1000

void analog_init(void) {
  DDRD |= (1 << PD4);
  TCCR3B |= (1 << WGM32);
  TCCR3A |= (1 << COM3A0);
  TCCR3B |= (1 << CS30);
  OCR3A = 0;
}

analog_input analog_read() {
  analog_input out;
  volatile uint8_t *adc = (uint8_t *)BASE_ADDR;
  adc[0] = (uint8_t)0b00000000; // Prob smth else
  _delay_us(30);                // works with a value as low as 0
  out.analog_ch0 = adc[0];
  out.analog_ch1 = adc[0];
  out.analog_ch2 = adc[0];
  out.analog_ch3 = adc[0];
  return out; // Get the value
}



