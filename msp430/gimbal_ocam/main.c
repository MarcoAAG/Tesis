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
int16_t error[2] = {0, 0};
int16_t errorAnte[2] = {0.0, 0.0};
int16_t Sumerror[2] = {0.0, 0.0};

float Kp[2] = {0.0, 0.0};
float Ki[2] = {0.0, 0.0};
float Kd[2] = {0.0, 0.0};

float controller[2] = {0.0, 0.0};
float duty[2] = {0.0, 0.0};

float LLSM1CW = 1.4;
float LHSM1CW = 0.5;
float LLSM1CCW = 1.6;
float LHSM1CCW = 2.5;

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
	WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

	configIO();

	while (INIT_FLAG == 0)
	{
		if ((P1IN & BIT3) == 0)
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

	while (1)
	{
		if (UART_FLAG == 1)
		{
			readCoordinates();
			UART_FLAG = 0;
		}
		if (TIMER_FLAG == 1)
		{
		    /*
			// Get errors
			error[0] = REFERENCE_X - CX;
			error[1] = REFERENCE_Y - CY;
			
			//* Set Kp Gain
			Kp[0] = 0.0;
			Kp[1] = 0.5;

			controller[0] = Kp[0] * ((float)error[0] / 1000.0);
			controller[1] = Kp[1] * ((float)error[1] / 1000.0);

			if(error[1] > 0)
			{
			    duty[1] = LLSM1CCW + controller[1];
			}
			if(error[1] == 0)
			{
			    duty[1] = 1.5;
			}
			if(error[1] < 0)
			{
			    duty[1] = LLSM1CW - controller[1];
			}

			if(duty[1] > LHSM1CCW)
			{
			    duty[1] = LHSM1CCW;
			}
			if(duty[1] < LHSM1CW)
			{
			    duty[1] = LHSM1CW;
			}

			pwm(1.5);
			*/

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
	P1OUT = 0; //!Setting all register to 0v
	P1SEL = 0;
	P1SEL2 = 0;
	P1DIR = 0;
	P1DIR |= BIT0 | BIT6;
	P1REN = BIT3;  //Enable pull up/down
	P1OUT |= BIT3; //Pin3 as pull-up
}

void readCoordinates(void)
{
	readChar(&cDato);
	strReceiver[i] = cDato;
	i++;
	//i = i>(length_array-1) ? 0 : i;
	if (i == length_array)
	{
		i = 0;
		FLAG_COMPLETE = 1;
		P1OUT ^= BIT6; // light up P1.6 LED on RX

		strtoint(&CX, &CY);

		//sciSendData((uint8_t *)&error[1], 2);
		//riteStr("\r\n");

        // Get errors
		errorAnte[0] = error[0];
		errorAnte[1] = error[1];

		error[0] = REFERENCE_X - CX;
        error[1] = REFERENCE_Y - CY;

        Sumerror[0] += error[0];
        Sumerror[1] += error[1];

        //* Set Kp Gain
        Kp[1] = 0.005;
        Ki[1] = 0.001;
        Kd[1] = 0.0005;

        controller[0] = Kp[0] * ((float)error[0] / 1000.0);
        controller[1] = Kp[1] * ((float)error[1] / 1000.0) +  Ki[1] * ((float)Sumerror[1] / 1000.0) + Kd[1]*(((float)error[1] / 1000.0) - ((float)errorAnte[1] / 1000.0)) ;

        if(error[1] > 0)
        {
            duty[1] = LLSM1CCW + controller[1];
        }
        if(error[1] == 0)
        {
            duty[1] = 1.5;
        }
        if(error[1] < 0)
        {
            duty[1] = LLSM1CW + controller[1];
        }

        if(duty[1] > LHSM1CCW)
        {
            duty[1] = LHSM1CCW;
        }
        if(duty[1] < LHSM1CW)
        {
            duty[1] = LHSM1CW;
        }

        pwm(duty[1]);
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
	// P1OUT ^= BIT6; // light up P1.6 LED on RX
	UART_FLAG = 1;
	IFG2 &= ~UCA0RXIFG; // Clear RX flag
}
