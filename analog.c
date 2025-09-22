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

joystick joystick_read(analog_input analog_in, bool digital_input, uint8_t max, uint8_t min, uint8_t zero) {
  joystick pos;
  uint8_t x_pos = analog_in.analog_ch1;
  uint8_t y_pos = analog_in.analog_ch0;

  //TODO needs to be changed according to the calibration struct
  if (x_pos > zero){
    pos.x_pos = (x_pos  - zero)*100/(max-zero);
    pos.direction_x = RIGHT;
  } else if (x_pos <  zero) {
    pos.x_pos = (x_pos - zero)*100/(min-zero);
    pos.direction_x = LEFT;
  } else {
    pos.x_pos = 0;
    pos.direction_x = NEUTRAL;
  }
 //TODO needs to be changed according to calobration struct 
if ( y_pos > zero){
    pos.y_pos = (y_pos- zero)*100/(max-zero);
    pos.direction_y = FORWARD;
  } else if (y_pos  < zero) {
    pos.y_pos = (y_pos  - zero)*100/(min-zero);
    pos.direction_y = BACKWARD;
  } else {
    pos.y_pos = 0;
    pos.direction_y = NEUTRAL;
  }

  return pos;
}

touchpad touchpad_read(analog_input analog_in){
  touchpad out;
  uint8_t xpos = analog_in.analog_ch2; 
  uint8_t ypos = analog_in.analog_ch3;
//TODO needs to take in calibration struct, and use it accordingly
  out.x_pos = ((xpos - min)*100)/max;
  out.y_pos = ((ypos - min)*100)/max;
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
