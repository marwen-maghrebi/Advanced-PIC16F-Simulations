
#ifndef SPI_H
#define SPI_H

#include "main.h"

// Define the CS pin
#define CS_TRIS TRISCbits.TRISC2  // Configure RC2 direction
#define CS_PIN PORTCbits.RC2      // Access RC2 pin directly

void SPI_Master_Init();
void SPI_Set_CS(bool state);     // Function to control CS pin
void SPI_Write(uint8_t Data);
uint8_t SPI_Read();              // Function to read data from SPI

#endif // SPI_H
