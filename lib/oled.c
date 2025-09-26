#include "oled.h"
#define SLAVE_SELECT PB4
#define COMMAND_DATA PB2

void oled_init()
{
    // Set D/C as output
    DDRB |= (1 << COMMAND_DATA);

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
    //spi_send_char(0xad, SLAVE_SELECT); //obsolete
    //spi_send_char(0x00, SLAVE_SELECT); //obsolete
    oled_write_command(DISPLAY_RESUME_RAM_CONTENT);
    oled_write_command(SET_DISPLAY_NOT_INVERTED);
    oled_write_command(DISPLAY_ON_NORMAL_MODE);
}

// void oled_reset() //this could be done by just using the pin to control the hardware reset
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

void oled_write_command(char command){

    PORTB &= ~(1 << COMMAND_DATA);
    spi_send_char(command, SLAVE_SELECT);
}

// void oled_print(char *data)
// {

// }

// void oled_set_brightness(uint8_t level)
// {

// }

