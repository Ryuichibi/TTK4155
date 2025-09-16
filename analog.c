#include "analog.h"


void clk_init(void){
  WGM01 = 3;
  COM00 = 2;
  OCR00 = 1;

}
