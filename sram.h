#ifndef SRAM_H
#define SRAM_H

#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>

void sram_init(void);
void sram_write(uint8_t data, uint16_t addr);
uint8_t sram_read(uint16_t addr);
void sram_test(void);

#endif