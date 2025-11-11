#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include "sam.h"

/*
 * Remember to update the Makefile with the (relative) path to the uart.c file.
 * This starter code will not compile until the UART file has been included in the Makefile. 
 * If you get somewhat cryptic errors referencing functions such as _sbrk, 
 * _close_r, _write_r, _fstat etc, you have most likely not done that correctly.

 * If you get errors such as "arm-none-eabi-gcc: no such file", you may need to reinstall the arm gcc packages using
 * apt or your favorite package manager.
 */
#include "lib/uart.h"
#include "lib/time.h"
#include "lib/can.h"
#include "lib/motor_controller.h"
#include "lib/controller.h"

#define F_CPU 84000000
#define F_CAN F_CPU/2

#define CDTY_L 0b1100001011111000u // This is 1ms
#define CDTY_H 0b1011100001101100u // This is 2ms 
#define CDTY_STEP_DIFF 0b11011u

// Controller
int16_t reference;
int16_t integral_sum = 0;
int16_t output;
int32_t x;
int16_t error;

// Game state
bool game_start = false;

void TC0_Handler()
{
    read_encoder(&x);
    error = reference - x;

    
    integral_sum += error;
    output = K_P * error + K_I * CONTROLLER_PERIOD * integral_sum;
    if (output >= 100){
        set_motor_speed(100, false);
    }else if (output <= -100) {
        set_motor_speed(100, true);
    }else if(output < 0){
        set_motor_speed(output * -1, true);
    } else {
        set_motor_speed(output, false);
    }
    // insert controller code here
    TC0->TC_CHANNEL[0].TC_SR;
    NVIC_ClearPendingIRQ(ID_TC0);
}




void score_update(int16_t score_delta)
{
    
}


int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer
    
    PIOB->PIO_PDR |= PIO_PDR_P13;
    PIOB->PIO_MDDR |= PIO_MDDR_P13;
    PIOB->PIO_ABSR |= PIO_ABSR_P13;

    PIOB->PIO_PER |= PIO_PER_P27; // Solenoid pin
    PIOB->PIO_OER |= PIO_OER_P27; // Solenoid pin

    PMC->PMC_PCER1 |= (1 << 4);
    motor_init();

    PWM->PWM_CLK |= 1;
    PWM->PWM_CLK |= (5 << 8);
    PWM->PWM_CH_NUM[1].PWM_CMR = 0b1011;
    PWM->PWM_CH_NUM[1].PWM_CPRD = 0b1100110100010100;
    PWM->PWM_CH_NUM[1].PWM_CDTY = CDTY_L;

    PWM->PWM_ENA |= 2;

    PMC->PMC_PCER1 |= (1 << 5);
    ADC->ADC_MR = 0;
    ADC->ADC_CHER = 1;
    ADC->ADC_IER = ADC_IER_COMPE;
    ADC->ADC_EMR = ADC_EMR_CMPMODE_LOW;
    ADC->ADC_CWR = ADC_CWR_LOWTHRES(200);

    encoder_init();


    //Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Hello World\n\r");

    can_init((CanInit){.brp = F_CPU/4000000-1, .phase1 = 5, .phase2 = 6, .propag = 1, .sjw = 3}, 0);

    CanMsg *msg = malloc(sizeof(CanMsg));
    CanMsg msg2;
    msg2.id = 2;
    msg2.length = 1;
    //Byte8 data = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    msg2.byte[0] = 0;
    msg2.byte[1] = 0;
    
    uint64_t time_last_solenoid = 0; 
    uint64_t time_last_score = 0;
    uint16_t score = 0;

    while (1)
    {
        ADC->ADC_CR = 2;
        // Delay
        //printf("Int %d\n", ((ADC->ADC_ISR & ADC_ISR_COMPE) >> 26));
        //printf("Over: %d\n", ADC->ADC_OVER);
        //printf("%d\n", (ADC->ADC_LCDR & 0xFFF));
        /*
        printf("%d\t", output);
        printf("%d\t", error);
        printf("%d\t", reference);
        printf("%d\n\r", x);
        //can_tx(msg2);
        */

        if (can_rx(msg) && msg->id == 3 && !game_start)
        {
            game_start = true;
            printf("Received message from ID 3\n");
        }

        //printf("%d\n", ADC->ADC_ISR & ADC_ISR_COMPE);
        if (game_start) // Game is running
        {
            can_rx(msg);
            can_printmsg(*msg);
            if (msg->id != 4) {
                continue;
            }

            //can_printmsg(*msg);
            uint16_t cdty_value = CDTY_L - (CDTY_STEP_DIFF * msg->byte[0]);
            PWM->PWM_CH_NUM[1].PWM_CDTYUPD = cdty_value;

            //printf("Joystick x: %d\n\r", msg->byte[0]);
            //printf("Joystick y: %d\n\r", msg->byte[1]);


            if (msg->byte[2] && ((time_now() - time_last_solenoid) > msecs(500))) // Push out the solenoid
            {
                PIOB->PIO_CODR |= PIO_CODR_P27; 
                time_last_solenoid = time_now();
            }

            if ((time_now() - time_last_solenoid) > msecs(70)) // Pull in the solenoid after 70 ms
            {
                PIOB->PIO_SODR |= PIO_SODR_P27; 
            }
           
            if ((time_now() - time_last_score) > seconds(1)) // Increase score every 1 s
            {
                score += 1;

                msg2.id = 2;
                msg2.length = 2;
                msg2.byte[0] = score & 0xFF;
                msg2.byte[1] = (score >> 8) & 0xFF;
                
                if((time_now() - time_last_solenoid) > msecs(70)) {
                can_tx(msg2);
                }

                time_last_score = time_now();
                //printf("Score: %d\n\r", score);
            }

                        reference = -(msg->byte[3]*60) ;
            //printf("Reference set to %d\n\r", reference);
        }
        if ((ADC->ADC_ISR & ADC_ISR_COMPE) && (score > 0) ){ //Checks if a goal is registered
                ADC->ADC_ISR; //Clear interrupt flag
                game_start = false; // End game
                score = 0;

                msg2.id = 1;
                msg2.length = 0;
                can_tx(msg2);
                printf("Goal!\n\r");
            }


    }
    
}
