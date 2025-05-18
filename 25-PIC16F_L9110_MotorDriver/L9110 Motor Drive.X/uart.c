
#include "uart.h"

void UART_TX_Init(void)
{
    // Recalculate baud rate generator for 16 MHz
    // For 9600 bps: SPBRG = (Fosc / (16 * Baud Rate)) - 1
    // (16000000 / (16 * 9600)) - 1 = 103
    BRGH = 1;       // High-speed baud rate
    SPBRG = 103;    // Updated SPBRG value for 16 MHz clock
    
    // Enable Asynchronous Serial Port
    SYNC = 0;
    SPEN = 1;
    
    // Set RX-TX Pins to be in UART mode
    TRISC6 = 0;     // TX pin (output)
    TRISC7 = 1;     // RX pin (input)
    
    // Enable UART Transmission
    TXEN = 1;
    // Enable UART Reception
    CREN = 1;
    }

uint8_t UART_TX_Empty(void) {
    return TRMT; // Return if transmitter buffer is empty
    }

void UART_Write(uint8_t data) {
    while (!UART_TX_Empty());
    TXREG = data; // Write data to TX register
    }

void UART_Write_Text(const char* text) {
    while (*text != '\0') {
        UART_Write(*text++);
        }
    }

