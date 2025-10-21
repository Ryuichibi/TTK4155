#include "joystick.h"

void send_joystick(joystick *joystick)
{
    can_message_t message;
    message.id = JOYSTICK_ID;
    message.data_count = 2;
    message.data[0] = joystick->x_pos;
    message.data[1] = joystick->y_pos;
    
    can_send(message);
    mcp2515_request_send(0x01);

    char *error;
    mcp2515_read(0x30, error);
    
    if (*error != 0x08) 
    {
        printf("%01x\n", *error);
    } 
    else 
    {
        printf("Joystick message sent\n");
    }
}

void read_joystick(joystick *joystick, analog_input analog_in)
{
    joystick->x_pos_raw = (uint8_t)analog_in.analog_ch1;
    joystick->y_pos_raw = (uint8_t)analog_in.analog_ch0;
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

void joystick_calibrate(joystick *joystick)
{
    printf("Joystick Calibration Started!\n");
    printf("Keep joystick neutral\n");

    joystick->parameters.x_neutral_max = 127;
    joystick->parameters.x_neutral_min = 127;
    joystick->parameters.y_neutral_max = 127;
    joystick->parameters.y_neutral_min = 127;

    _delay_ms(2000);

    for (uint8_t i = 0; i < 10; i++) {
        read_joystick(joystick, analog_read());
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