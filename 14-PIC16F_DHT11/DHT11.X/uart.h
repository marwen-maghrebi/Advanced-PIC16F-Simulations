
/*
 *  * File: uart.h
 *  * Author: Marwen
 *  * Comments:
 *  * Revision history:
 *  */

#ifndef UART_H
#define UART_H

#include <xc.h>
#include <stdint.h>

void UART_TX_Init(void);
uint8_t UART_TX_Empty(void);
void UART_Write(uint8_t data);
void UART_Write_Text(const char* text);
uint8_t UART_Read(void);
void UART_Read_Text(char* buffer, uint8_t max_length);

#endif // UART_H
