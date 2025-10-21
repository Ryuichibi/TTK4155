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

int main()
{
    SystemInit();

    WDT->WDT_MR = WDT_MR_WDDIS; //Disable Watchdog Timer

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
        /* code */
        // PIOB->PIO_SODR = PIO_SODR_P13;
        // time_spinFor(msecs(1000));
        // PIOB->PIO_CODR = PIO_CODR_P13;
        // time_spinFor(msecs(1000));

        //can_tx(msg2);
        if (can_rx(msg))
        {
            can_printmsg(*msg);
        }

    }
    
}