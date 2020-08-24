/*
 * uart.h
 *
 *  Created on: Jul 27, 2020
 *      Author: marco
 */

#ifndef UART_H_
#define UART_H_

#pragma once

#include "stdint.h"

void uartInit(void);
void readChar(char *receive);
void writeChar(char c);
void writeStr(char *str);
void sciSendData(uint8_t *text, uint32_t length);

#endif /* UART_H_ */
