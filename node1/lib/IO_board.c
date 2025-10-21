#include "IO_board.h"
#include "analog.h"
#include "spi.h"
#include <stdint.h>
#include <util/delay.h>

void read_touchpad(touchpad *touchpad)
{
    char *flushBuffer;
    spi_open_com(SSIO_board);
    spi_send_char(0x01);
    _delay_us(40);
    spi_receive_char(&(touchpad->x_pos_raw)); // read x position of toucpad
    _delay_us(2);
    spi_receive_char(&(touchpad->y_pos_raw)); // read y position of toucpad
    _delay_us(2);
    spi_receive_char(&(touchpad->size)); // read size of touchpad area
    spi_close_com(SSIO_board);

    touchpad->x_pos = (uint8_t)touchpad->x_pos_raw * 100 / 255;
    touchpad->y_pos = (uint8_t)touchpad->y_pos_raw * 100 / 255;
}

void read_slider(slider *slider)
{
    spi_open_com(SSIO_board);
    spi_send_char(0x02);
    _delay_us(40);
    spi_receive_char(&slider->x_pos_raw);
    _delay_us(2);
    spi_receive_char(&slider->size);
    spi_close_com(SSIO_board);

    slider->x_pos = (slider->x_pos_raw * 100) / 255;
}


void read_buttons(buttons *buttons)
{
    spi_open_com(SSIO_board);
    spi_send_char(0x04);
    _delay_us(40);
    spi_receive_char(&buttons->right);
    _delay_us(2);
    spi_receive_char(&buttons->left);
    _delay_us(2);
    spi_receive_char(&buttons->nav);
    spi_close_com(SSIO_board);
}

void digital_write_led(char led, char value)
{
    spi_open_com(SSIO_board);
    spi_send_char(0x05);
    _delay_us(40);
    spi_send_char(led);
    _delay_us(2);
    spi_send_char(value);
    spi_close_com(SSIO_board);
}

void analog_write_led(char led, char value)
{
    spi_open_com(SSIO_board);
    spi_send_char(0x06);
    _delay_us(40);
    spi_send_char(led);
    _delay_us(2);
    spi_send_char(value);
    spi_close_com(SSIO_board);
}
