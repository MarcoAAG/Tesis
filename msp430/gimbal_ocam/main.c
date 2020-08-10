#include <msp430g2553.h>
#include "clock.h"
#include "uart.h"
#include "ros.h"

int INIT_FLAG = 0;

void configIO(void);

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	configIO();

	while(INIT_FLAG == 0)
	{
	    if((P1IN & BIT3) == 0)
	    {
	        INIT_FLAG = 1;
	    }
	}

	P1OUT |= BIT0;

	return 0;
}

void configIO(void)
{
    P1OUT = 0;           //!Setting all register to 0v
    P1SEL = 0;
    P1SEL2 = 0;
    P1DIR = 0;
    P1DIR |= BIT0 | BIT6;
    P1REN = BIT3;       //Enable pull up/down
    P1OUT |= BIT3;      //Pin3 as pull-up
}
