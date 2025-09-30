#include "oled.h"
#define SLAVE_SELECT PB4
#define COMMAND_DATA PB2

void oled_init()
{
    // Set D/C as output
    DDRB |= (1 << COMMAND_DATA);

    spi_send_char(0xae, SLAVE_SELECT);
    spi_send_char(0xa0, SLAVE_SELECT);
    spi_send_char(0xda, SLAVE_SELECT);
    spi_send_char(0x12, SLAVE_SELECT);
    spi_send_char(0xc0, SLAVE_SELECT);
    spi_send_char(0xa8, SLAVE_SELECT);
    spi_send_char(0x3f, SLAVE_SELECT);
    spi_send_char(0xd5, SLAVE_SELECT);
    spi_send_char(0x80, SLAVE_SELECT);
    spi_send_char(0x81, SLAVE_SELECT);
    spi_send_char(0xF0, SLAVE_SELECT);
    spi_send_char(0xd9, SLAVE_SELECT);
    spi_send_char(0x21, SLAVE_SELECT);
    spi_send_char(0x20, SLAVE_SELECT);
    spi_send_char(0x02, SLAVE_SELECT);
    spi_send_char(0xdb, SLAVE_SELECT);
    spi_send_char(0x30, SLAVE_SELECT);
    spi_send_char(0xad, SLAVE_SELECT);
    spi_send_char(0x00, SLAVE_SELECT);
    spi_send_char(0xa4, SLAVE_SELECT);
    spi_send_char(0xa6, SLAVE_SELECT);
    //spi_send_char(0xb4, SLAVE_SELECT); // Select page
    //spi_send_char(0x21, SLAVE_SELECT);
    //spi_send_char(0x00, SLAVE_SELECT);
    //spi_send_char(0x7f, SLAVE_SELECT);
    spi_send_char(0xaf, SLAVE_SELECT);
}

void oled_reset()
{
    for (uint8_t i = 0; i < 8; i++)
    {
        oled_clear_row(i);
    }

    //TODO: Do more?
}

// void oled_home()
// {

// }

void oled_goto_row(uint8_t row)
{
    PORTB &= ~(1 << COMMAND_DATA);

    uint8_t command = 0xb0;
    command |= (row & 0x0f);
    spi_send_char(command, SLAVE_SELECT);
}

void oled_goto_column(uint8_t column)
{
    PORTB &= ~(1 << COMMAND_DATA);

    uint8_t lower_column_command = 0x00;
    uint8_t upper_column_command = 0x10;

    lower_column_command |= (column & 0x0f);
    upper_column_command |= ((column & 0xf0) >> 4);

    spi_send_char(lower_column_command, SLAVE_SELECT);
    spi_send_char(upper_column_command, SLAVE_SELECT);

}

void oled_clear_row(uint8_t row)
{
    oled_goto_row(row);

    PORTB |= (1 << COMMAND_DATA);

    for (uint8_t i = 0; i < 128; i++)
    {
        spi_send_char(0x00, SLAVE_SELECT);
    }

    PORTB &= ~(1 << COMMAND_DATA);

}

void oled_position(uint8_t row, uint8_t column)
{
    oled_goto_row(row);
    oled_goto_column(column);

}

void oled_write_data(char data)
{

    PORTB |= (1 << COMMAND_DATA);
    spi_send_char(data, SLAVE_SELECT);
    PORTB &= ~(1 << COMMAND_DATA);
    
}

// void oled_print(char *data)
// {

// }

void oled_set_brightness(uint8_t level)
{
    PORTB &= ~(1 << COMMAND_DATA);

    spi_send_char(0x81, SLAVE_SELECT);
    spi_send_char(level, SLAVE_SELECT);
}

