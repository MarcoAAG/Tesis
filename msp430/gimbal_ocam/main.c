#include <msp430g2553.h>
#include "clock.h"
#include "uart.h"
#include "ros.h"
#include <stdint.h>

#define length_array 6
#define REFERENCE_X 320
#define REFERENCE_Y 240

/*
**********************************************************************************************************************************************************
*   WRITE GLOBAL VARIABLES BELOW
**********************************************************************************************************************************************************
*/
// Flags for interrupts
int INIT_FLAG = 0;
int UART_FLAG = 0;
int TIMER_FLAG = 0;
int FLAG_COMPLETE = 0;

// UART stuff
char cDato = 0;
char strReceiver[length_array] = {0};
unsigned int i = 0;

// Variables for storage coordinates from ROS
uint16_t CX;
uint16_t CY;

// Control stuff
uint16_t error[2] = {0,0};
float Kp[2] = {0.0, 0.0};
float controller[2] = {0.0, 0.0};

/*
**********************************************************************************************************************************************************
*   WRITE FUNCTION PROTOTYPE BELOW
**********************************************************************************************************************************************************
*/
void configIO(void);
void readCoordinates(void);
void strtoint(uint16_t *_x, uint16_t *_y);

/*
**********************************************************************************************************************************************************
*   MAIN
**********************************************************************************************************************************************************
*/
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

	//All interrupts are enabled
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
	        // convert char to int
	        //strtoint(&CX, &CY);

	        // Get errors
	        error[0] = REFERENCE_X - CX;
	        error[1] = REFERENCE_Y - CY;

	        Kp[0] = 0.2;

	        controller[0] = Kp[0] * error[0];
	        controller[1] = Kp[1] * error[1];

	        pwm(1.4);
	        TIMER_FLAG = 0;
	    }
	}
}

/*
**********************************************************************************************************************************************************
*   WRITE FUNCTION BODY BELOW
**********************************************************************************************************************************************************
*/
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
        FLAG_COMPLETE = 1;
        strtoint(&CX, &CY);
        //writeStr2(strReceiver, length_array);
        //writeStr("\r\n");
        sciSendData((uint8_t *)&error[1],2);
        writeStr("\r\n");
    }
}
void strtoint(uint16_t *_x, uint16_t *_y)
{
    *_x = (((uint16_t)strReceiver[0] - 48) * 100) + (((uint16_t)strReceiver[1] - 48) * 10) + ((uint16_t)strReceiver[2] - 48);
    *_y = (((uint16_t)strReceiver[3] - 48) * 100) + (((uint16_t)strReceiver[4] - 48) * 10) + ((uint16_t)strReceiver[5] - 48);
}



/*
**********************************************************************************************************************************************************
*   WRITE INTERRUPTS BELOW
**********************************************************************************************************************************************************
*/

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
