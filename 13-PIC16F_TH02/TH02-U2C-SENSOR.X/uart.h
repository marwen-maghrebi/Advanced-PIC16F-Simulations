
//uart.h
#ifndef UART_H
#define UART_H
#include "i2c.h"

void UART_TX_Init(void);
uint8_t UART_TX_Empty(void);
void UART_Write(uint8_t data);
void UART_Write_Text(const char* text);

#endif // UART_H
