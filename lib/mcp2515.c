#include "mcp2515.h"

uint8_t mcp2515_init() 
{
    uint8_t status;

    mcp2515_reset();

    _delay_ms(1);

    mcp2515_read(MCP2515_CANSTAT, &status);
    if ((status & MCP2515_MODE_MASK) != MCP2515_CONFIG_MODE) 
    {
        printf("Fail\n");
        return 1;
    }

    // More?
    // Set mode?

    return 0;
}

void mcp2515_read(uint8_t address, uint8_t* data) 
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP2515_READ);
    spi_send_char(address);
    spi_receive_char(data);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_write(uint8_t address, uint8_t data)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP2515_WRITE);
    spi_send_char(address);
    spi_send_char(data);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_request_send(uint8_t transmit_buffers)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    uint8_t command = transmit_buffers | MCP2515_RTS;

    spi_send_char(command);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_read_status(uint8_t* status)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP2515_STATUS);
    spi_receive_char(status);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP2515_BIT_MODIFY);
    spi_send_char(address);
    spi_send_char(mask);
    spi_send_char(data);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_reset()
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP2515_RESET);

    spi_close_com(MCP2515_SLAVE_SELECT);
}