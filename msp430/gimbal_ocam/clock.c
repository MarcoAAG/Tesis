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
    TA0CCR0 = 65000-1;
    TA0CCTL0 = 0x16;
}


void pwmInit(void)
{
    P2DIR |= BIT2;
    P2SEL |= BIT2;
    TA1CCR0 = 40000 -1;
    TA1CCTL1 = OUTMOD_7;
    TA1CTL = TASSEL_2 + MC_1 + ID_3;
}

void pwm(float duty_ms)
{
    int duty = 2000 * duty_ms;
    TA1CCR1 = duty;
}
