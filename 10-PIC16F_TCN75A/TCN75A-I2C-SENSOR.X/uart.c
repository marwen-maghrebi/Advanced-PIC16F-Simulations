
// uart.c
#include "uart.h"
#include <stdlib.h> // For atoi function to convert text to integer

void UART_TX_Init(void) {
    BRGH = 1;       // High-speed baud rate
    SPBRG = 51;     // Set SPBRG for 9600 bps with 8 MHz clock
    SYNC = 0;       // Asynchronous mode
    SPEN = 1;       // Enable serial port
    TRISC6 = 0;     // TX pin (output)
    TRISC7 = 1;     // RX pin (input)
    TXEN = 1;       // Enable UART Transmission
    CREN = 1;       // Enable UART Reception
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

uint8_t UART_Read(void) {
    while (!RCIF);  // Wait until data is received
    return RCREG;   // Read received data
    }

void UART_Read_Text(char* buffer, uint8_t max_length) {
    uint8_t i = 0;
    char received_char;
    do {
        received_char = UART_Read();
        if (received_char != '\n' && received_char != '\r') {
                buffer[i++] = received_char;
        }
    } while (received_char != '\n' && received_char != '\r' && i < max_length - 1);
            buffer[i] = '\0'; // Null-terminate the string
}
    
int UART_Read_Temperature(void) {
    char buffer[10]; // Buffer for incoming temperature string
    UART_Read_Text(buffer, sizeof(buffer)); // Read text until newline

    // Convert received text to integer
    int temperature = atoi(buffer); // atoi`converts string to integer, handling any sign
    return temperature;
    }
    
