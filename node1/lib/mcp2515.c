#include "mcp2515.h"

uint8_t mcp2515_init()
{
    uint8_t status;

    mcp2515_reset();

    _delay_ms(1);

    mcp2515_read(MCP_CANSTAT, &status);
    if ((status & MODE_POWERUP) != MODE_CONFIG) {
        printf("Failed to enter config mode\n");
        return 1;
    }
    char CNF1 = SJW4 | BRP;
    char CNF2 = PRSEG | PHSEG | SAMPLE_3X | BTLMODE_ON;
    char CNF3 = SOF_DISABLE | WAKFIL_DISABLE | PHSEG2;

    mcp2515_write(MCP_CNF1, CNF1);
    mcp2515_write(MCP_CNF2, CNF2);
    mcp2515_write(MCP_CNF3, CNF3);

    //mcp2515_bit_modify(MCP_CANINTE, 0x01, 0x01); // Enable interrupt for receiving
    mcp2515_write(MCP_CANINTE, 0x01);
    
    mcp2515_bit_modify(MCP_RXB0CTRL, 0x60, 0xFF); //mask is set to full "dont care"
    mcp2515_bit_modify(MCP_RXB1CTRL, 0x60, 0xFF); //mask is set to full "dont care"

    mcp2515_write(MCP_CANCTRL, MODE_NORMAL);
    mcp2515_read(MCP_CANSTAT, &status);
    if(status != 0){
        printf("Failed to enter normal mode\n");
        return 1;
  } else {
        printf("Entered normal mode\n");
  }

    // Interrupt for CAN controller. Used for receiving CAN messages
    //SREG |= (1 << 7); // Enable interrupts
    //EMCUCR &= ~(0x01); // Falling edge for INT2
    //GICR |= (1 << 5); // Interupt INT2

    return 0;
}

void mcp2515_read(uint8_t address, uint8_t *data)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP_READ);
    spi_send_char(address);
    spi_receive_char(data);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_write(uint8_t address, uint8_t data)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP_WRITE);
    spi_send_char(address);
    spi_send_char(data);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_sequential_write(uint8_t address, uint8_t *data, uint8_t n_entries)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP_WRITE);
    spi_send_char(address);
    
    for (uint8_t i = 0; i < n_entries; i++) 
    {
        spi_send_char(data[i]);
    }

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_request_send(uint8_t transmit_buffers)
{
    spi_open_com(MCP2515_SLAVE_SELECT);
    uint8_t command = transmit_buffers | MCP_RTS_BASE;
    spi_send_char(command);
    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_read_status(uint8_t *status)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP_READ_STATUS);
    spi_receive_char(status);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_bit_modify(uint8_t address, uint8_t mask, uint8_t data)
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP_BITMOD);
    spi_send_char(address);
    spi_send_char(mask);
    spi_send_char(data);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

void mcp2515_reset()
{
    spi_open_com(MCP2515_SLAVE_SELECT);

    spi_send_char(MCP_RESET);

    spi_close_com(MCP2515_SLAVE_SELECT);
}

