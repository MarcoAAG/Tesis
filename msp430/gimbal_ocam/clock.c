/*
 * clock.c
 *
 *  Created on: Jul 27, 2020
 *      Author: marco
 */

#include <msp430g2553.h>
#include "clock.h"

void clockInit(void)
{
    //* ajustamos el registro de control a 16MHz
    DCOCTL = CALDCO_16MHZ;

    //* Ajustamos el registro basico de control a 16MHz
    BCSCTL1 = CALBC1_16MHZ;
}

void timerAInit(void)
{
    TA0CTL = TASSEL_2 | ID_3 | MC_1;// | TACLR;
    TA0CCR0 = 31999;
    TA0CCTL0 = 0x16;
}
