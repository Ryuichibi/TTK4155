#include "IO_board.h"
#include "analog.h"
#include "spi.h"
#include <stdint.h>
#include <util/delay.h>

#ifndef USE_ADC
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

    touchpad->x_pos = touchpad->x_pos_raw * 100 / 255;
    touchpad->y_pos = touchpad->y_pos_raw * 100 / 255;
}
#else

void read_touchpad(touchpad *touchpad, analog_input analog_in)
{
    char *flushBuffer;
    spi_open_com(SSIO_board);
    spi_send_char(0x01);
    _delay_us(40);
    spi_receive_char(flushBuffer); // read x position of toucpad
    _delay_us(2);
    spi_receive_char(flushBuffer); // read y position of toucpad
    _delay_us(2);
    spi_receive_char(&(touchpad->size)); // read size of touchpad area
    spi_close_com(SSIO_board);

    touchpad->x_pos = analog_in.analog_ch2;
    touchpad->y_pos = analog_in.analog_ch3;
    //  out.x_pos = ((xpos - 0)*100)/255;
    //  out.y_pos = ((ypos - 0)*100)/255;
}
#endif

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

#ifndef USE_ADC
void read_joystick(joystick *joystick)
{
    spi_open_com(SSIO_board);
    spi_send_char(0x03);
    _delay_us(40);
    spi_receive_char(&joystick->x_pos_raw);
    _delay_us(2);
    spi_receive_char(&joystick->y_pos_raw);
    _delay_us(2);
    spi_receive_char(&joystick->button);
    spi_close_com(SSIO_board);

    uint8_t x_zero = (joystick->parameters.x_neutral_max +
                      joystick->parameters.x_neutral_min) /
                     2;
    uint8_t y_zero = (joystick->parameters.y_neutral_max +
                      joystick->parameters.y_neutral_min) /
                     2;

    // autocalibrate if the raw value is outside the earlier max-min range
    if (joystick->x_pos_raw > joystick->parameters.x_max) {
        joystick->parameters.x_max = joystick->x_pos_raw;
    }
    if (joystick->x_pos_raw < joystick->parameters.x_min) {
        joystick->parameters.x_min = joystick->x_pos_raw;
    }

    if (joystick->y_pos_raw > joystick->parameters.y_max) {
        joystick->parameters.y_max = joystick->y_pos_raw;
    }
    if (joystick->y_pos_raw < joystick->parameters.y_min) {
        joystick->parameters.y_min = joystick->y_pos_raw;
    }

    // scaling and transltaing values to enum values
    if (joystick->x_pos_raw > joystick->parameters.x_neutral_max) {
        joystick->x_pos = (joystick->x_pos_raw - x_zero) * 100 /
                          (joystick->parameters.x_max - x_zero);
        joystick->direction_x = LEFT;
    } else if (joystick->x_pos < joystick->parameters.x_neutral_min) {
        joystick->x_pos = (joystick->x_pos_raw - x_zero) * -100 /
                          (joystick->parameters.x_min - x_zero);

        joystick->direction_x = RIGHT;
    } else {
        joystick->x_pos = 0;
        joystick->direction_x = NEUTRALX;
    }

    if (joystick->y_pos_raw > joystick->parameters.y_neutral_max) {
        joystick->y_pos = (joystick->y_pos_raw - y_zero) * 100 /
                          (joystick->parameters.y_max - y_zero);
        joystick->direction_y = BACKWARD;
    } else if (joystick->y_pos_raw < joystick->parameters.y_neutral_min) {
        joystick->y_pos = (joystick->y_pos_raw - y_zero) * -100 /
                          (joystick->parameters.y_min - y_zero);
        joystick->direction_y = FORWARD;
    } else {
        joystick->y_pos = 0;
        joystick->direction_y = NEUTRALY;
    }
}
#else
void joystick_read(joystick *joystick, analog_input analog_in)
{
    joystick->x_pos_raw = analog_in.analog_ch1;
    joystick->y_pos_raw = analog_in.analog_ch0;
    // maybe the following should be moved to the calibration function, to avoid
    // unnecesairy calculations
    uint8_t x_zero = (joystick->parameters.x_neutral_max +
                      joystick->parameters.x_neutral_min) /
                     2;
    uint8_t y_zero = (joystick->parameters.y_neutral_max +
                      joystick->parameters.y_neutral_min) /
                     2;

    if (joystick->x_pos_raw > joystick->parameters.x_max) {
        joystick->parameters.x_max = joystick->x_pos_raw;
    }
    if (joystick->x_pos_raw < joystick->parameters.x_min) {
        joystick->parameters.x_min = joystick->x_pos_raw;
    }

    if (joystick->y_pos_raw > joystick->parameters.y_max) {
        joystick->parameters.y_max = joystick->y_pos_raw;
    }
    if (joystick->y_pos_raw < joystick->parameters.y_min) {
        joystick->parameters.y_min = joystick->y_pos_raw;
    }

    // scaling and transltaing values to enum values
    if (joystick->x_pos_raw > joystick->parameters.x_neutral_max) {
        joystick->x_pos = (joystick->x_pos_raw - x_zero) * 100 /
                          (joystick->parameters.x_max - x_zero);
        joystick->direction_x = LEFT;
    } else if (joystick->x_pos < joystick->parameters.x_neutral_min) {
        joystick->x_pos = (joystick->x_pos_raw - x_zero) * -100 /
                          (joystick->parameters.x_min - x_zero);

        joystick->direction_x = RIGHT;
    } else {
        joystick->x_pos = 0;
        joystick->direction_x = NEUTRALX;
    }

    if (joystick->y_pos_raw > joystick->parameters.y_neutral_max) {
        joystick->y_pos = (joystick->y_pos_raw - y_zero) * 100 /
                          (joystick->parameters.y_max - y_zero);
        joystick->direction_y = BACKWARD;
    } else if (joystick->y_pos_raw < joystick->parameters.y_neutral_min) {
        joystick->y_pos = (joystick->y_pos_raw - y_zero) * -100 /
                          (joystick->parameters.y_min - y_zero);
        joystick->direction_y = FORWARD;
    } else {
        joystick->y_pos = 0;
        joystick->direction_y = NEUTRALY;
    }
}

#endif

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

void joystick_calibrate(joystick *joystick)
{
    printf("Joystick Calibration Started!\n");
    printf("Keep joystick neutral\n");

    _delay_ms(2000);

    for (uint8_t i = 0; i < 10; i++) {
#ifndef USE_ADC
        joystick_read(joystick);
#else
        joystick_read(joystick, analog_read());
#endif
        if (joystick->y_pos_raw < joystick->parameters.y_neutral_min) {
            joystick->parameters.y_neutral_min = joystick->y_pos_raw - 2;
        }

        if (joystick->y_pos_raw > joystick->parameters.y_neutral_max) {
            joystick->parameters.y_neutral_max = joystick->y_pos_raw + 2;
        }

        if (joystick->x_pos_raw < joystick->parameters.x_neutral_min) {
            joystick->parameters.x_neutral_min = joystick->x_pos_raw - 2;
        }

        if (joystick->x_pos_raw > joystick->parameters.x_neutral_max) {
            joystick->parameters.x_neutral_max = joystick->x_pos_raw + 2;
        }

        _delay_ms(10);
    }

    //    printf("Keep joystick down\n");
    //
    //    _delay_ms(5000);
    //
    //    for (uint8_t i = 0; i < 10; i++) {
    //        analog_input data = analog_read();
    //
    //        if (data.analog_ch0 > calib_parameters->y_max) {
    //            calib_parameters->y_max = data.analog_ch0;
    //        }
    //
    //        _delay_ms(10);
    //    }
    //
    //    printf("Keep joystick up\n");
    //
    //    _delay_ms(5000);
    //
    //    for (uint8_t i = 0; i < 10; i++) {
    //        analog_input data = analog_read();
    //
    //        if (data.analog_ch0 < calib_parameters->y_min) {
    //            calib_parameters->y_min = data.analog_ch0;
    //        }
    //
    //        _delay_ms(10);
    //    }
    //
    //    printf("Keep joystick left\n");
    //
    //    _delay_ms(5000);
    //
    //    for (uint8_t i = 0; i < 10; i++) {
    //        analog_input data = analog_read();
    //
    //        if (data.analog_ch1 > calib_parameters->x_max) {
    //            calib_parameters->x_max = data.analog_ch1;
    //        }
    //
    //        _delay_ms(10);
    //    }
    //
    //    printf("Keep joystick right\n");
    //
    //    _delay_ms(5000);
    //
    //    for (uint8_t i = 0; i < 10; i++) {
    //        analog_input data = analog_read();
    //
    //        if (data.analog_ch1 < calib_parameters->x_min) {
    //            calib_parameters->x_min = data.analog_ch1;
    //        }
    //
    //        _delay_ms(10);
    //    }
}
