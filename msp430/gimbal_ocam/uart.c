/*
 * uart.c
 *
 *  Created on: Jul 27, 2020
 *      Author: marco
 */

#include <msp430g2553.h>
#include "uart.h"

#define RX BIT1
#define TX BIT2

void uartInit(void)
{
    P1SEL |= RX | TX;
    P1SEL2 |= RX | TX;

    //* reiniciamos el puerto uart
    UCA0CTL1 = UCSWRST;

    //* Configuracion del registro para modo UART asincrono
    UCA0CTL0 = 0;

    //* Configuracion del registro para modo UART no loopback
    UCA0STAT = 0;

    //* Configuracion del registro para establecer la fuente de reloj MCLK
    UCA0CTL1 = UCSSEL_2;

    //*Establecemos valor de baud rate a 9600
    UCA0BR0 = 104; //! Se obtuvo valor de hoja de datos
    UCA0BR1 = 0;   //!Se obtuvo valor de hoja de datos

    UCA0MCTL = UCOS16;
    UCA0CTL1 &= ~UCSWRST;
    IE2 |= UCA0RXIE;
}

void writeChar(char c)
{
    while (!(IFG2 & UCA0TXIFG))
        ;
    UCA0TXBUF = c;
}

void writeStr(char *str)
{
    while (*str)
    {
        writeChar(*str);
        str++;
    }
}
void writeStr2(char *str, int length)
{
    int i=0;
    for(i =0; i<length; i++)
    {
        writeChar(str[i]);
    }
}

void readChar(char *receive)
{
    while (IFG2 & UCA0RXIFG == 0)
        ;
    *receive = UCA0RXBUF;
}

void readstr(char *receive_str, int length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        // while (IFG2 & UCA0RXIFG == 0)
        // ;
        // receive_str[i] = UCA0RXBUF;
        readChar(receive_str[i]);
    }
}

/*
#pragma vector = USCIAB0RX_VECTOR
__interrupt void ReceiveInterrupt(void)
{
    P1OUT ^= BIT6;      // light up P1.6 LED on RX
    IFG2 &= ~UCA0RXIFG; // Clear RX flag
}
*/
