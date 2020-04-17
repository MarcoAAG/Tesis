/** @file HL_sys_main.c 
*   @brief Application main file
*   @date 11-Dec-2018
*   @version 04.07.01
*
*   This file contains an empty main function,
*   which can be used for the application.
*/

/* 
* Copyright (C) 2009-2018 Texas Instruments Incorporated - www.ti.com  
* 
* 
*  Redistribution and use in source and binary forms, with or without 
*  modification, are permitted provided that the following conditions 
*  are met:
*
*    Redistributions of source code must retain the above copyright 
*    notice, this list of conditions and the following disclaimer.
*
*    Redistributions in binary form must reproduce the above copyright
*    notice, this list of conditions and the following disclaimer in the 
*    documentation and/or other materials provided with the   
*    distribution.
*
*    Neither the name of Texas Instruments Incorporated nor the names of
*    its contributors may be used to endorse or promote products derived
*    from this software without specific prior written permission.
*
*  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
*  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
*  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
*  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
*  OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
*  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
*  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
*  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
*  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
*  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
*  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/


/* USER CODE BEGIN (0) */
/* USER CODE END */

/* Include Files */

#include "HL_sys_common.h"

/* USER CODE BEGIN (1) */
#include "HL_sys_core.h"
#include "FreeRTOS.h"
#include "os_task.h"
#include "HL_het.h"
#include "HL_sci.h"
#include "HL_spi.h"
#include "as5048.h"
// #include "stdio.h"
/* USER CODE END */

/** @fn void main(void)
*   @brief Application main function
*   @note This function is empty by default.
*
*   This function is called after startup.
*   The user can use this function to implement the application.
*/

/* USER CODE BEGIN (2) */

/*
**************************************************************************************
FUNCTIONS FOR PWM CREATOR
**************************************************************************************
*/
static const uint32 s_het1pwmPolarity[8U] =
    {
        3U,
        3U,
        3U,
        3U,
        3U,
        3U,
        3U,
        3U,
};
hetSIGNAL_t pwm0het0; // pwm signal for motor input
hetSIGNAL_t pwm1het1; // pwm signal for motor input
void setpwmsignal(hetRAMBASE_t *hetRAM, uint32 pwm, hetSIGNAL_t signal);

/*
**************************************************************************************
FUNCTION FOR FREERTOS SYSTEM
**************************************************************************************
*/
static void TaskInit(void *pvParameters);
static void TaskControl(void *pvParameters);
TaskHandle_t TaskInitHandle;

/*
**************************************************************************************
FUNCTION FOR SCI COMMUNICATION
**************************************************************************************
*/
void sciSendText(sciBASE_t *sci, uint8 *text, uint32 length);
void sciSendData(sciBASE_t *sci, uint8 *text, uint32 length);
#define TSIZE_INTRO 9
uint8 TEXT1[TSIZE_INTRO] = {'C', 'O', 'N', 'E', 'C', 'T', 'A', 'D', 'O'};
unsigned char command[80];

/*
**************************************************************************************
FUNCTION FOR SPI COMMUNICATION
**************************************************************************************
*/
uint16 AS5048A_CLEAR_ERROR_FLAG1[1] = {0x0001};
uint16 AS5048A_PROGRAMMING_CONTROL1[1] = {0x0003};
uint16 AS5048A_OTP_REGISTER_ZERO_POS_HIGH1[1] = {0x0016};
uint16 AS5048A_OTP_REGISTER_ZERO_POS_LOW1[1] = {0x0017};
uint16 AS5048A_DIAG_AGC1[1] = {0x3FFD};
uint16 AS5048A_MAGNITUDE1[1] = {0x3FFE};
uint16 AS5048A_ANGLE1[1] = {0x3FFF};
uint16 AS5048A_NOP[1] = {0x0000};
spiDAT1_t SPI1_data_configCh0;

uint16_t ComandoSPI1[1] = {0};

/* USER CODE END */

int main(void)
{
/* USER CODE BEGIN (3) */
    hetInit();
    sciInit();
    spiInit();
    // as5048Init();

    SPI1_data_configCh0.CS_HOLD = FALSE;   //defines whether or not you want to hold the chip select at the end of one single SPI transfer.
    SPI1_data_configCh0.WDEL = TRUE;       //specified the duration of delay at the end of the current transaction before starting the next one.
    SPI1_data_configCh0.DFSEL = SPI_FMT_0; // antes en SPI_FMT_1: modo 2: Select the Data word format by setting DFSEL bits. Select the Number of the configured SPIFMTx register (0 to 3) to used for the communication. Note: It is highly recommended to use SPIDAT1 register, SPIDAT0 is supported for compatibility reason
    SPI1_data_configCh0.CSNR = 0xFE;       // defines which chip select you want to activate

    _enable_interrupt_(); // habilitamos interrupci�n global

    if (xTaskCreate(TaskInit, "TaskInit", configMINIMAL_STACK_SIZE, NULL, 2, &TaskInitHandle) != pdTRUE)
    {
        // TASK HAS NOT CREATED
        return 0;
    }
    vTaskStartScheduler();
    for (;;)
        ;
    /* USER CODE END */

    return 0;
}


/* USER CODE BEGIN (4) */
/*
**************************************************************************************
FUNCTION FOR FREERTOS SYSTEM
**************************************************************************************
*/
static void TaskInit(void *pvParameters)
{
    portTickType xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();

    sciSendText(sciREG1, &TEXT1[0], TSIZE_INTRO); /* send text 1 */

    //PWM0 init
    pwm0het0.period = 20000;
    pwm0het0.duty = 200;
    setpwmsignal(hetRAM1, pwm0, pwm0het0);
    //PWM1 init
    pwm1het1.period = 20000;
    pwm1het1.duty = 0;
    setpwmsignal(hetRAM1, pwm1, pwm1het1);

    vTaskDelayUntil(&xLastWakeTime, (5000 * portTICK_RATE_MS)); //Sleep task for 2 seconds
    if (xTaskCreate(TaskControl, "TaskControl", configMINIMAL_STACK_SIZE, NULL, 1, NULL) != pdTRUE)
    {
        // TASK HAS NOT CREATED
    }
    vTaskDelete(TaskInitHandle);
}
static void TaskControl(void *pvParameters)
{
    TickType_t xLastExecutionTime;
    uint16 angle[1] = {0};
    uint16 readData[1] = {0};

    float raw_angle;
    uint16 Data;
    uint16 error;

    int rotation;
    const TickType_t xFrequency = 5;
    // read(AS5048A_ANGLE1, readData, spiREG1, &SPI1_data_configCh0);
    uint16 command[1] = {0b0100000000000000}; // PAR=0 R/W=R
    AS5048A_ANGLE1[0] = AS5048A_ANGLE1[0] | command[0];
    spiSendAndGetData(spiREG1, &SPI1_data_configCh0, 1, AS5048A_ANGLE1, readData);
    xLastExecutionTime = xTaskGetTickCount();

    for (;;)
    {

        pwm0het0.duty = 400;
        setpwmsignal(hetRAM1, pwm0, pwm0het0);
        pwm1het1.duty = 700;
        setpwmsignal(hetRAM1, pwm1, pwm1het1);
        spiSendAndGetData(spiREG1, &SPI1_data_configCh0, 1, AS5048A_ANGLE1, readData);
        delaymio(100);
        spiSendAndGetData(spiREG1, &SPI1_data_configCh0, 1, AS5048A_NOP, readData);

        Data = readData[0] & ~0xC000;
        // raw_angle = (float)(Data);
        // raw_angle = raw_angle * 360.0 / 8192.0;
        //    data = data & 0b0100000000000000;

        sciSendData(sciREG1, (uint8 *)&Data, 2);
        sciSend(sciREG1, 2, (unsigned char *)"\r\n");

        vTaskDelayUntil(&xLastExecutionTime, 10 * portTICK_PERIOD_MS);
    }
}
void vApplicationIdleHook(void)
{
}
/*
**************************************************************************************
FUNCTIONS FOR PWM CREATOR
**************************************************************************************
*/
void setpwmsignal(hetRAMBASE_t *hetRAM, uint32 pwm, hetSIGNAL_t signal)
{
    uint32 action;
    uint32 pwmPolarity = 0U;
    float64 pwmPeriod = 0.0F;

    if (hetRAM == hetRAM1)
    {
        pwmPeriod = (signal.period * 1000.0F) / 800.000F;
        pwmPolarity = s_het1pwmPolarity[pwm];
    }
    else
    {
    }
    if (signal.duty == 0U)
    {
        action = (pwmPolarity == 3U) ? 0U : 2U;
    }
    else if (signal.duty >= 10000U)
    {
        action = (pwmPolarity == 3U) ? 2U : 0U;
    }
    else
    {
        action = pwmPolarity;
    }

    hetRAM->Instruction[(pwm << 1U) + 41U].Control = ((hetRAM->Instruction[(pwm << 1U) + 41U].Control) & (~(uint32)(0x00000018U))) | (action << 3U);
    hetRAM->Instruction[(pwm << 1U) + 41U].Data = ((((uint32)pwmPeriod * signal.duty) / 10000) << 7U) + 128U; // el duty va de 0 a 10000, es decir de 0ms a 20ms
    hetRAM->Instruction[(pwm << 1U) + 42U].Data = ((uint32)pwmPeriod << 7U) - 128U;
}

/*
**************************************************************************************
FUNCTION FOR SCI COMMUNICATION
**************************************************************************************
*/
void sciSendData(sciBASE_t *sci, uint8 *text, uint32 length)
{
    uint8 txt = 0;
    uint8 txt1 = 0;

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

        while ((sciREG1->FLR & 0x4) == 4)
            ;                       /* wait until busy */
        sciSendByte(sciREG1, txt1); /* send out text   */
        while ((sciREG1->FLR & 0x4) == 4)
            ;                      /* wait until busy */
        sciSendByte(sciREG1, txt); /* send out text   */
    };
}

void sciSendText(sciBASE_t *sci, uint8 *text, uint32 length)
{
    while (length--)
    {
        while ((sciREG1->FLR & 0x4) == 4)
            ;                          /* wait until busy */
        sciSendByte(sciREG1, *text++); /* send out text   */
    };
}

/* USER CODE END */
