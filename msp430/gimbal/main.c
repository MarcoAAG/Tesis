#include <msp430g2553.h>
#include "clock.h"
#include "uart.h"
#include "ros.h"
#include <stdint.h>

/*
 * ******************************************************************************************************************************
 * MACROS
 * ******************************************************************************************************************************
 */
#define XREFERENCE 320
#define YREFERENCE 240

/*
 * ******************************************************************************************************************************
 * VARIABLES GLOBALES
 * ******************************************************************************************************************************
 */

int TIMER_FLAG = 0;
int UART_FLAG = 0;

int COUNT;
uint16_t X;  //! Variable donde se guardara la coordenada X
uint16_t Y;  //! Variable donde se guardara la coordenada Y
uint16_t CX; //! Variable temporal para hacer recorrimiento de bits
uint16_t CY; //! Variable temporal para hacer recorrimiento de bits

// variables de control
int16_t ERROR[2] = {0, 0};
int16_t LAST_ERROR[2] = {0, 0};
int16_t SUM_ERROR[2] = {0.0};
volatile double DUTY[2] = {0.0, 0.0};
volatile double KP[2] = {0.0, 0.0};
volatile double KD[2] = {0.0, 0.0};
volatile double KI[2] = {0.0, 0.0};
volatile double CTRL[2] = {0.0, 0.0};

/*
 * ******************************************************************************************************************************
 * PROTOTIPO DE FUNCIONES
 * ******************************************************************************************************************************
 */
void configIO(void);
void readCoordinates(void);
void getErrors(int16_t *out_error, int16_t *out_last_error, int16_t *out_sum_error);
void control(volatile double *out_ctrl);
void signo(volatile double *out_ctrl);
void getDuty(volatile double *out_duty);

void main(void)
{
    WDTCTL = WDTPW | WDTHOLD; // stop watchdog timer

    configIO();
    while ((P1IN & BIT3) != 0)
        ;

    clockInit();
    timerAInit();
    uartInit();
    pwmInit();

    //! Habilitamos las interrupciones
    _enable_interrupt();

    initROS();

    while (1)
    {
        if (UART_FLAG)
        {
            readCoordinates();
            UART_FLAG = 0;
        }

        if (TIMER_FLAG)
        {
            if (Y == 240)
            {
                P1OUT ^= BIT0;
            }
            getErrors(ERROR, LAST_ERROR, SUM_ERROR);
            KP[1] = 0.1;
            KD[1] = 0.0;
            KI[1] = 0.0001;
            control(CTRL);
            signo(CTRL);
            getDuty(DUTY);
            pwm(DUTY[1]);
            TIMER_FLAG = 0;
        }
    }
}

/*
 * CUERPO DE LAS FUNCIONES
 */
void configIO(void)
{
    P1OUT = 0; //!Configurar el registro de salida a 0v
    P1SEL = 0;
    P1SEL2 = 0;
    P1DIR = 0;
    P1DIR |= BIT0 | BIT6; //! P1.0 y P1.6 salidas
    P1REN = BIT3;         //! Habiltamos pull up/down
    P1OUT |= BIT3;        //! P1.3 como pull-up
}

void readCoordinates(void)
{
    char cDato;
    readChar(&cDato);

    COUNT++;
    switch (COUNT)
    {
    case 1:
    {
        uint16_t temp_y;
        temp_y = (uint16_t)cDato;
        temp_y = temp_y << 8;
        CY = temp_y;
        break;
    }
    case 2:
        CX = (uint16_t)cDato;
        break;

    case 3:
    {
        uint16_t temp_x;
        temp_x = (uint16_t)cDato;
        temp_x = temp_x << 8;
        CX |= temp_x;
        break;
    }
    case 4:
        CY |= (uint16_t)cDato;
        Y = CY;
        X = CX;

        COUNT = 0;
        break;
    }
}

void getErrors(int16_t *out_error, int16_t *out_last_error, int16_t *out_sum_error)
{
    *out_last_error = *out_error;
    *out_error = XREFERENCE - X;
    *out_sum_error += *out_error;

    out_error++;
    out_last_error++;
    out_sum_error++;

    *out_last_error = *out_error;
    *out_error = YREFERENCE - Y;
    *out_sum_error += *out_error;
}

void control(volatile double *out_ctrl)
{
    volatile double ctrlP; //! control proporcional
    volatile double ctrlD; //! control derivativo
    volatile double ctrlI; //! control integral

    ctrlP = KP[0] * ERROR[0];
    ctrlD = KD[0] * (ERROR[0] - LAST_ERROR[0]); //! se divide entre el tiempode muestreo (en segundos)
    ctrlI = KI[0] * SUM_ERROR[0];
    *out_ctrl = ctrlP + ctrlD + ctrlI;

    out_ctrl++;

    ctrlP = KP[1] * ERROR[1];
    ctrlD = KD[1] * (ERROR[1] - LAST_ERROR[1]); //! se divide entre el tiempode muestreo (en segundos)
    ctrlI = KI[1] * SUM_ERROR[1];
    *out_ctrl = ctrlP + ctrlD + ctrlI;
}

void signo(volatile double *out_ctrl)
{
    //! TODO: sentido para motor 2 coordenada X
    //* codigo

    out_ctrl++;

    *out_ctrl /= 100000;

    if (*out_ctrl > 0)
    {
        *out_ctrl += 1.55;
    }
    if (*out_ctrl == 0)
    {
        *out_ctrl = 1.5;
    }
    if (*out_ctrl < 0)
    {
        *out_ctrl = *out_ctrl + 1.4;
    }
}
void getDuty(volatile double *out_duty)
{
    //! TODO: limites para motor 2 coordenada X
    //* codigo

    out_duty++;

    if (CTRL[1] > 2.5)
    {
        CTRL[1] = 2.5;
    }
    if (CTRL[1] < 0.4)
    {
        CTRL[1] = 0.4;
    }
    *out_duty = CTRL[1];
}

/*
**********************************************************************************************************************************************************
*   ISR
**********************************************************************************************************************************************************
*/
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A_CCRO_ISR(void)
{
    // P1OUT ^= BIT0;
    TIMER_FLAG = 1;
}

#pragma vector = USCIAB0RX_VECTOR
__interrupt void ReceiveInterrupt(void)
{
    UART_FLAG = 1;
    IFG2 &= ~UCA0RXIFG; // Clear RX flag
}
