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
    spi_send_char(0xb0, SLAVE_SELECT); // Select page
    spi_send_char(0x21, SLAVE_SELECT);
    spi_send_char(0x08, SLAVE_SELECT);
    spi_send_char(0x17, SLAVE_SELECT);
    spi_send_char(0xaf, SLAVE_SELECT);
}

// void oled_reset()
// {

// }

// void oled_home()
// {

// }

// void oled_goto_line(uint8_t line)
// {

// }

// void oled_clear_line(uint8_t line)
// {

// }

// void oled_position(uint8_t row, uint8_t column)
// {

// }

void oled_write_data(char data)
{

    PORTB |= (1 << COMMAND_DATA);
    spi_send_char(data, SLAVE_SELECT);
    PORTB &= ~(1 << COMMAND_DATA);
    
}

// void oled_print(char *data)
// {

// }

// void oled_set_brightness(uint8_t level)
// {

// }

