#ifndef MCP2515_H
#define MCP2515_H

#define F_CPU 4915200
#include <util/delay.h>
#include <avr/io.h>
#include <stdio.h>
#include <stdbool.h>
#include "spi.h"

uint8_t mcp2515_init();
void mcp2515_read(uint8_t address, uint8_t* data);
void mcp2515_write(uint8_t address, uint8_t data);
void mcp2515_request_send(uint8_t transmit_buffers);
void mcp2515_read_status(uint8_t* status);
void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data);
void mcp2515_reset();



#define MCP2515_SLAVE_SELECT PB4


// Instructions
#define MCP2515_READ 0x03
#define MCP2515_RESET 0xC0
#define MCP2515_WRITE 0x02
#define MCP2515_RTS 0x80
#define MCP2515_STATUS 0xA0
#define MCP2515_BIT_MODIFY 0x05

// Modes
#define MCP2515_MODE_MASK 0xE0
#define MCP2515_NORMAL_MODE 0x00
#define MCP2515_SLEEP_MODE 0x20
#define MCP2515_LOOPBACK_MODE 0x40
#define MCP2515_LISTEN_ONLY_MODE 0x60
#define MCP2515_CONFIG_MODE 0x80

// Registers
#define MCP2515_CANSTAT 0x0E

#endif