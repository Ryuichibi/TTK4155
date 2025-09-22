#include "analog.h"

#define BASE_ADDR 0x1000


void analog_init(void){
  DDRD |= (1 << PD4);
  TCCR3B |= (1 <<WGM32);
  TCCR3A |= (1 << COM3A0);
  TCCR3B |= (1 << CS30);
  OCR3A  = 0;

}


analog_input analog_read() {
    analog_input out;
    volatile uint8_t *adc = (uint8_t *) BASE_ADDR;
    adc[0] = (uint8_t) 0b00000000 ; // Prob smth else
    _delay_us(30); //works with a value as low as 0
    out.analog_ch1 = adc[0];
    out.analog_ch2 = adc[0];
    out.analog_ch3 = adc[0];
    out.analog_ch4 = adc[0];
    return out; // Get the value
}


void joystick_read(struct joystick *position){
  char channelY = 0;
  char channelX = 1;

}

void joystick_calibrate(calib_parameters *calib_parameters) {

  printf("Joystick Calibration Started!\n");
  printf("Keep joystick neutral");

  _delay_ms(2000);

  for (uint8_t i = 0; i < 10; i++) {
    analog_input data = analog_read();

    if (data.analog_ch0 < calib_parameters->y_neutral_min) 
    {
      calib_parameters->y_neutral_min = data.analog_ch0;
    }

    if (data.analog_ch0 > calib_parameters->y_neutral_max) 
    {
      calib_parameters->y_neutral_max = data.analog_ch0;
    }

    if (data.analog_ch1 < calib_parameters->x_neutral_min) 
    {
      calib_parameters->x_neutral_min = data.analog_ch1;
    }

    if (data.analog_ch1 > calib_parameters->x_neutral_max) 
    {
      calib_parameters->x_neutral_max = data.analog_ch1;
    }

    _delay_ms(10);

  }

  printf("Keep joystick in max y position");

  _delay_ms(2000);

  for (uint8_t i = 0; i < 10; i++) {
    analog_input data = analog_read();

    if (data.analog_ch0 > calib_parameters->y_max) 
    {
      calib_parameters->y_max = data.analog_ch0;
    }

    _delay_ms(10);
  }

  printf("Keep joystick in min y position");

  _delay_ms(2000);

  for (uint8_t i = 0; i < 10; i++) {
    analog_input data = analog_read();
    
    if (data.analog_ch0 < calib_parameters->y_min) 
    {
      calib_parameters->y_min = data.analog_ch0;
    }

    _delay_ms(10);
  }

  printf("Keep joystick in max x position");

  _delay_ms(2000);

  for (uint8_t i = 0; i < 10; i++) {
    analog_input data = analog_read();
    
    if (data.analog_ch1 > calib_parameters->x_max) 
    {
      calib_parameters->x_max = data.analog_ch1;
    }

    _delay_ms(10);
  }

  printf("Keep joystick in min x position");

  _delay_ms(2000);

  for (uint8_t i = 0; i < 10; i++) {
    analog_input data = analog_read();
    
    if (data.analog_ch1 < calib_parameters->x_min) 
    {
      calib_parameters->x_min = data.analog_ch1;
    }

    _delay_ms(10);
  }
  
}
