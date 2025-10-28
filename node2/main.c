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

#define F_CPU 84000000
#define F_CAN F_CPU/2

#define CDTY_L 0b1100001011111000u // This is 1ms
#define CDTY_H 0b1011100001101100u // This is 2ms 
#define CDTY_STEP_DIFF 0b11011u

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

    PIOB->PIO_PDR |= PIO_PDR_P13;
    PIOB->PIO_MDDR |= PIO_MDDR_P13;
    PIOB->PIO_ABSR |= PIO_ABSR_P13;

    PMC->PMC_PCER1 |= (1 << 4);
    PWM->PWM_CLK |= 1;
    PWM->PWM_CLK |= (5 << 8);
    PWM->PWM_CH_NUM[1].PWM_CMR = 0b1011;
    PWM->PWM_CH_NUM[1].PWM_CPRD = 0b1100110100010100;
    PWM->PWM_CH_NUM[1].PWM_CDTY = CDTY_L;
    PWM->PWM_ENA |= 2;



    //Uncomment after including uart above
    uart_init(F_CPU, 9600);
    printf("Hello World\n\r");

    can_init((CanInit){.brp = F_CPU/4000000-1, .phase1 = 5, .phase2 = 6, .propag = 1, .sjw = 3}, 0);

    CanMsg *msg = malloc(sizeof(CanMsg));
    CanMsg msg2;
    msg2.id = 47;
    msg2.length = 8;
    Byte8 data = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h'};
    msg2.byte8 = data;

    while (1)
    {

        //can_tx(msg2);
        if (can_rx(msg))
        {
            can_printmsg(*msg);
        }
        uint16_t cdty_value = CDTY_L - (CDTY_STEP_DIFF * msg->byte[0]);
        PWM->PWM_CH_NUM[1].PWM_CDTYUPD = cdty_value;

    }
    
}