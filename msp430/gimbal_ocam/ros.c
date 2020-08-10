/*
 * ros.c
 *
 *  Created on: Jul 29, 2020
 *      Author: marco
 */

#include "ros.h"
#include "uart.h"

void initROS(void)
{
    const char data_init = '@';
    char data_init_receive = 0;

    writeChar(data_init);
    readChar(&data_init_receive);
    while(data_init_receive != 'a')
    {
        writeChar(data_init);
        readChar(&data_init_receive);
    }
}


