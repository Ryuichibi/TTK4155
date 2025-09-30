#include "oled.h"
#define SLAVE_SELECT PB4
#define COMMAND_DATA PB2
#include "../font.h"
void oled_init()
{
    // Set D/C as output
    DDRB |= (1 << COMMAND_DATA);

<<<<<<< HEAD
    oled_write_command(DISPLAY_OFF);
    oled_write_command(SET_SEGMENT_REMAP);
    oled_write_command(SET_COM_PINS);
    oled_write_command(ALTERNATIVE_COM_CONFIG);
    oled_write_command(SET_COM_OUTPUT_DIRECTION);
    oled_write_command(SET_MULTIPLEX_RATIO);
    oled_write_command(MULTIPLEX_RATIO);
    oled_write_command(SET_DISPLAY_CLOCK);
    oled_write_command(DISPLAY_CLOCK);
    oled_write_command(SET_CONTRAST_CONTROL);
    oled_write_command(CONTRAST);
    oled_write_command(SET_PRECHARGE_PERIOD);
    oled_write_command(PRECHARGE_PERIOD);
    oled_write_command(SET_MEMORY_ADDRESSING_MODE);
    oled_write_command(MEMORY_MODE_PAGE);
    oled_write_command(SET_VCOMH_LEVEL);
    oled_write_command(VCOMH_LEVEL);
    // spi_send_char(0xad, SLAVE_SELECT); //obsolete
    // spi_send_char(0x00, SLAVE_SELECT); //obsolete
    oled_write_command(DISPLAY_RESUME_RAM_CONTENT);
    oled_write_command(SET_DISPLAY_NOT_INVERTED);
    oled_write_command(DISPLAY_ON_NORMAL_MODE);
=======
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
>>>>>>> a2a85f8ecbc024e6d0803ce030328cba76b96425
}

void oled_reset()
{
<<<<<<< HEAD
    for (uint8_t i = 0; i < 8; i++) {
        oled_clear_row(i);
    }

    // TODO: Do more?
=======
    for (uint8_t i = 0; i < 8; i++)
    {
        oled_clear_row(i);
    }

    //TODO: Do more?
>>>>>>> a2a85f8ecbc024e6d0803ce030328cba76b96425
}

// void oled_home()
// {

// }

void oled_goto_row(uint8_t row)
<<<<<<< HEAD
=======
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
>>>>>>> a2a85f8ecbc024e6d0803ce030328cba76b96425
{
    char row_char = SET_PAGE | row;
    oled_write_command(row_char);
}

void oled_goto_column(uint8_t column)
{
    char lower = (0x0F & column) | SET_LOWER_COLUMN_START_ADDRESS;
    char higher = (column >> 4) | SET_HIGHER_COLUMN_START_ADDRESS;
    oled_write_command(lower);
    oled_write_command(higher);
}

void oled_clear_row(uint8_t row)
{
    oled_goto_row(row);
    oled_goto_column(0);
    for (uint8_t i = 0; i <= 127; i++) {
        oled_goto_column(i);
        oled_write_data(0xFF);
        // TODO this is wrong, and is here only for testing
    }
}



void oled_position(uint8_t row, uint8_t column)
{
    oled_goto_row(row);
    oled_goto_column(column);
}

void oled_write_data(volatile char data)
{
    PORTB |= (1 << COMMAND_DATA);
    spi_send_char(data, SLAVE_SELECT);
    PORTB &= ~(1 << COMMAND_DATA);
}

void oled_write_command(volatile char command)
{
    PORTB &= ~(1 << COMMAND_DATA);
    spi_send_char(command, SLAVE_SELECT);
}

<<<<<<< HEAD
void oled_print_letter(char data)
{
    for (uint8_t i = 0; i < 8; i++) {
        char encoded_data = pgm_read_byte(&font8[(
            int)data][i]); // probably needs an offset of 20 for the encoding
                           // to work properly, but it should print something
                           // that looks like an alphanumerical character
        oled_write_data(encoded_data);
    }
=======
// }

void oled_set_brightness(uint8_t level)
{
    PORTB &= ~(1 << COMMAND_DATA);

    spi_send_char(0x81, SLAVE_SELECT);
    spi_send_char(level, SLAVE_SELECT);
>>>>>>> a2a85f8ecbc024e6d0803ce030328cba76b96425
}

void oled_set_brightness(uint8_t level)
{
    oled_write_command(SET_CONTRAST_CONTROL);
    oled_write_command((char)level);
}
