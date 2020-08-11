/*
 * uart.c
 *
 *  Created on: Jul 27, 2020
 *      Author: marco
 */

#include <msp430g2553.h>
#include "uart.h"
#include "stdint.h"

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

void sciSendData(uint8_t *text, uint32_t length)
{
    uint8_t txt = 0;
    uint8_t txt1 = 0;

#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
    text = text + (length - 1);
#endif

    while (length--)
    {
#if ((__little_endian__ == 1) || (__LITTLE_ENDIAN__ == 1))
        txt = *text--;
#else
        txt = *text++;
#endif

        txt1 = txt;

        txt &= ~(0xF0);
        txt1 &= ~(0x0F);
        txt1 = txt1 >> 4;

        if (txt <= 0x9)
        {
            txt += 0x30;
        }
        else if (txt > 0x9 && txt <= 0xF)
        {
            txt += 0x37;
        }
        else
        {
            txt = 0x30;
        }

        if (txt1 <= 0x9)
        {
            txt1 += 0x30;
        }
        else if ((txt1 > 0x9) && (txt1 <= 0xF))
        {
            txt1 += 0x37;
        }
        else
        {
            txt1 = 0x30;
        }
        writeChar(txt1);
        writeChar(txt);
    };
}
