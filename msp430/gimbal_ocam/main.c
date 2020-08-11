#include <msp430g2553.h>
#include "clock.h"
#include "uart.h"
#include "ros.h"
#include <stdint.h>

#define length_array 6


int INIT_FLAG = 0;
int UART_FLAG = 0;
int TIMER_FLAG = 0;

char cDato = 0;
char strReceiver[length_array] = {0};
unsigned int i = 0;

uint16_t CX;
uint16_t CY;


void configIO(void);
void readCoordinates(void);
void strtoint(uint16_t *_x, uint16_t *_y);

void main(int argc, char const *argv[])
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

	clockInit();

	pwmInit();


	timerAInit();
	uartInit();

	//All interrupts are enable
	_enable_interrupt();

	initROS();

	while(1)
	{
	    if(UART_FLAG == 1)
	    {
	        readCoordinates();
	        UART_FLAG = 0;
	    }
	    if(TIMER_FLAG == 1)
	    {
	        strtoint(&CX, &CY);
	        pwm(1.6);
	        TIMER_FLAG = 0;
	    }
	}
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

void readCoordinates(void)
{
    readChar(&cDato);
    strReceiver[i] = cDato;
    i++;
    //i = i>(length_array-1) ? 0 : i;
    if(i == length_array)
    {
        i = 0;
        writeStr2(strReceiver, length_array);
        writeStr("\r\n");
    }
}
void strtoint(uint16_t *_x, uint16_t *_y)
{
    *_x = (((uint16_t)strReceiver[0] - 48) * 100) + (((uint16_t)strReceiver[1] - 48) * 10) + ((uint16_t)strReceiver[2] - 48);
    *_y = (((uint16_t)strReceiver[3] - 48) * 100) + (((uint16_t)strReceiver[4] - 48) * 10) + ((uint16_t)strReceiver[5] - 48);
}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCRO_ISR(void)
{
    P1OUT ^= BIT0;
    TIMER_FLAG = 1;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void ReceiveInterrupt(void)
{
    P1OUT ^= BIT6; // light up P1.6 LED on RX
    UART_FLAG = 1;
    IFG2 &= ~UCA0RXIFG; // Clear RX flag
}
