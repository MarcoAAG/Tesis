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

//freeRTOS stuff
#include "FreeRTOS.h"
#include "os_task.h"

//GPIOS stuff
#include "HL_sci.h"
#include "HL_het.h"
#include "HL_gio.h"

//C stuff
#include "stdio.h"
#include "stdlib.h"

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
*FUNCTION FOR SCI COMMUNICATION
**************************************************************************************
*/
void sciSendText(sciBASE_t *sci, uint8 *text, uint32 length);
void sciSendData(sciBASE_t *sci, uint8 *text, uint32 length);

#define TSIZE_OK 2
uint8 TEXTOK[TSIZE_OK] = {'O', 'K'};
unsigned char DataReceived[8]; //Memory to storage coordinates
unsigned char ReceivedX[3];    //Memory to storage X coordinate
unsigned char ReceivedY[3];    //Memory to storage X coordinate

/*
**************************************************************************************
*FUNCTIONS FOR PWM CREATOR
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
*FUNCTION FOR FREERTOS SYSTEM
**************************************************************************************
*/
static void TaskInit(void *pvParameters);
static void TaskControl(void *pvParameters);
static void TaskDataAcquisition(void *pvParameters);
TaskHandle_t TaskInitHandle;

/*
**************************************************************************************
*FUNCTION FOR CONTROLLER
**************************************************************************************
*/
uint16 X = 0;
uint16 Y = 0;
#define REFERENCEX 320
#define REFERENCEY 240
bool getSign(float32 PID);
void sendPWM(bool dir, float32 PID, bool ID);

/* USER CODE END */

int main(void)
{
    /* USER CODE BEGIN (3) */
    _enable_IRQ(); // Enable the IRQ

    hetInit(); // Initialize the HET driver
    gioInit(); // Initialize the GIO driver
    sciInit(); // Initialize the SCI driver to 115200 baud

    if (xTaskCreate(TaskInit, "TaskInit", configMINIMAL_STACK_SIZE, NULL, 3, &TaskInitHandle) != pdTRUE)
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
*FUNCTION FOR FREERTOS SYSTEM
**************************************************************************************
*/
static void TaskInit(void *pvParameters)
{
    uint32 datareceived;

    gioSetBit(gioPORTB, 6, 0);
    gioSetBit(gioPORTB, 7, 0);

    datareceived = sciReceiveByte(sciREG1); //read serial
    while (datareceived != 64)
    {
        datareceived = sciReceiveByte(sciREG1); //Await for @
    }
    sciSendText(sciREG1, &TEXTOK[0], TSIZE_OK); /* send Response */
    sciSend(sciREG1, 2, (unsigned char *)"\r\n");

    gioSetBit(gioPORTB, 6, 1); //Led indication

    //PWM0 init
    pwm0het0.period = 20000;
    pwm0het0.duty = 0;
    setpwmsignal(hetRAM1, pwm0, pwm0het0);
    //PWM1 init
    pwm1het1.period = 20000;
    pwm1het1.duty = 0;
    setpwmsignal(hetRAM1, pwm1, pwm1het1);
    sciReceive(sciREG1, 6, (unsigned char *)&DataReceived); // Await furter character
    // sciReceive(sciREG1, 6, (unsigned char *)&DataReceived); // Await furter character
    if (xTaskCreate(TaskControl, "TaskControl", configMINIMAL_STACK_SIZE, NULL, 2, NULL) != pdTRUE)
    {
        // TASK HAS NOT CREATED
    }
    vTaskDelete(TaskInitHandle);
}
static void TaskControl(void *pvParameters)
{
    /*
    *MOTOR 1    <<<< Y >>>>
    * R       CENTRO        L
    * 343-670   671-723   723-1000
    * LEFT -> DOWN
    * RIGHT -> UP
    */

    /*
    *MOTOR 2    <<<< X >>>>
    * R         CENTRO        L
    * 402-674   675-726     727-1000
    */

    // Create a variable to get tick
    TickType_t xLastExecutionTime;
    xLastExecutionTime = xTaskGetTickCount();

    //local variable for coordinates
    uint16 coordinateX;
    uint16 coordinateY;

    /* error vector */
    float32 error[2] = {0.0, 0.0};     /* error X -> [0], error Y -> [1]*/
    float32 lasterror[2] = {0.0, 0.0}; /* error X -> [0], error Y -> [1]*/
    float32 sumerror[2] = {0.0, 0.0};  /* error X -> [0], error Y -> [1]*/
    /* Gain variables*/
    float32 Kp[2] = {0.0, 0.0}; /* X -> [0],  Y -> [1]*/
    float32 Ki[2] = {0.0, 0.0}; /* X -> [0],  Y -> [1]*/
    float32 Kd[2] = {0.0, 0.0}; /* X -> [0],  Y -> [1]*/

    float32 PID[2] = {0.0, 0.0}; /* X -> [0],  Y -> [1]*/

    for (;;)
    {

        gioSetBit(gioPORTB, 7, gioGetBit(gioPORTB, 7) ^ 1); //toogle a led

        /* data update*/
        coordinateX = X;
        coordinateY = Y;

        /*save last error*/
        lasterror[0] = error[0];
        lasterror[1] = error[1];

        /* get erro */
        error[0] = coordinateX - REFERENCEX;
        error[1] = coordinateY - REFERENCEY;

        /* sum errors*/
        sumerror[0] += error[0];
        sumerror[1] += error[1];

        /* setting gains */
        Kp[0] = 1.0;
        Kp[1] = 1.0;
        Ki[0] = 0.0;
        Ki[1] = 0.0;
        Kd[0] = 10.0;
        Kd[1] = 0.0;

        PID[0] = Kp[0] * error[0] + Ki[0] * sumerror[0] + Kd[0] * ((error[0] - lasterror[0]) * 0.02);
        PID[1] = Kp[1] * error[1] + Ki[1] * sumerror[1] + Kd[1] * ((error[1] - lasterror[1]) * 0.02);

        sendPWM(getSign(PID[1]), PID[1], 0);

        //vTaskDelayUntil(&xLastExecutionTime, 20 * portTICK_PERIOD_MS); //sleep task for specific time
    }
}
void vApplicationIdleHook(void)
{
}

/*
**************************************************************************************
*FUNCTION FOR CONTROLLER
**************************************************************************************
*/
bool getSign(float32 PID)
{
    bool ret;
    ret = (PID > 0) ? 1 : 0;
    return ret;
}
void sendPWM(bool dir, float32 PID, bool ID)
{
    /*
    *ID 0 -> MOTOR 1
    *ID 1 -> MOTOR 2
    *dir 0 -> TURN LEFT -> NEGATIVE ERROR
    *dir 1 -> TURN  RIGHT -> POSITIVE ERROR
    */

    //SETTING BOUNDARIES
    if (ID == 0)
    {
        if (dir == 1)
        {
            PID = 722 + (0.1) * PID;
            PID = (PID > 1000) ? 1000 : PID;
            PID = (PID < 727) ? 720 : PID;
        }
        else
        {

            PID = 671 + (0.1) * PID;
            PID = (PID > 670) ? 672 : PID;
            PID = (PID < 393) ? 393 : PID;
        }
        pwm0het0.duty = (uint32)PID; //For motor 1
        setpwmsignal(hetRAM1, pwm0, pwm0het0);
    }
}

/*
**************************************************************************************
*FUNCTIONS FOR PWM CREATOR
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
*FUNCTION FOR SCI COMMUNICATION
**************************************************************************************
*/
/*  This function execute every single interruption  */
void sciNotification(sciBASE_t *sci, unsigned flags)
{
    int i = 0;

    for (i = 0; i < 3; i++)
    {
        ReceivedX[i] = DataReceived[i];
        ReceivedY[i] = DataReceived[i + 3];
    }

    X = atoi((const char *)&ReceivedX);

    Y = atoi((const char *)&ReceivedY);

    //sciSendData(sciREG1, (uint8 *)&X, 2);
    //sciSend(sciREG1, 2, (unsigned char *)"\r\n");

    sciReceive(sci, 6, (unsigned char *)&DataReceived); // Await furter character
}
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
