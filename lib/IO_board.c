#include "IO_board.h"
#include "spi.h"
#include <util/delay.h>

void read_touchpad(touchpad *touchpad){
    char *flushBuffer;
    spi_open_com(SSIO_board);
    spi_send_char(0x01, SSIO_board);
    _delay_us(40);
    spi_receive_char(flushBuffer, SSIO_board);  //read x position of toucpad
    _delay_us(5);
    spi_receive_char(flushBuffer, SSIO_board);  //read y position of toucpad
    _delay_us(2);
    spi_receive_char(&(touchpad->size), SSIO_board); //read size of touchpad area
    spi_close_com(SSIO_board);
}

void read_slider(slider *slider){
    spi_open_com(SSIO_board);
    spi_send_char(0x02, SSIO_board);
    _delay_us(40);
    spi_receive_char(&slider->x_pos, SSIO_board);
    _delay_us(2);
    spi_receive_char(&slider->size, SSIO_board); 
    spi_close_com(SSIO_board);
}

void read_buttons(buttons *buttons){
    spi_open_com(SSIO_board);
    spi_send_char(0x04, SSIO_board);
    _delay_us(40);
    spi_receive_char(&buttons->right, SSIO_board);
    _delay_us(2);
    spi_receive_char(&buttons->left, SSIO_board);
    _delay_us(2);
    spi_receive_char(&buttons->nav, SSIO_board); 
    spi_close_com(SSIO_board);
}

void digital_write_led(char led, char value){
    spi_open_com(SSIO_board);
    spi_send_char(0x05, SSIO_board);
    _delay_us(40);
    spi_send_char(led, SSIO_board);
    _delay_us(2);
    spi_send_char(value, SSIO_board);
    spi_close_com(SSIO_board);
}

void analog_write_led(char led, char value){
    spi_open_com(SSIO_board);
    spi_send_char(0x06, SSIO_board);
    _delay_us(40);
    spi_send_char(led, SSIO_board);
    _delay_us(2);
    spi_send_char(value, SSIO_board);
    spi_close_com(SSIO_board);
}
