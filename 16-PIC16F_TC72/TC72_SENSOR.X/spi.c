
#include "spi.h"

void SPI_Master_Init() {
    SSPCON = 0b00100000;  // SSPM3:SSPM0 = 0000 (SPI Master mode, Fosc/64)
    SSPSTAT = 0b00000000; // CKE = 0 (Transmit occurs on rising edge), SMP = 0 (Input data sampled at middle)
    CKP = 0; // Idle clock state is low
    CKE = 0; // Data is transmitted on rising edge (active-to-idle)
    SSPEN = 1; // Enable SPI Module
    
    TRISC5 = 0; // SDO -> Output
    TRISC4 = 1; // SDI -> Input
    TRISC3 = 0; // SCK -> Output
    
    CS_TRIS = 0; // Set RC2 as output
    CS_PIN = 1;  // Set CS high (deselect device)
    }


void SPI_Write(uint8_t Data) {
    SSPBUF = Data;             // Transfer data to SPI buffer
    while (!SSPSTATbits.BF);   // Wait until transmission is complete
    }

uint8_t SPI_Read() {
    SSPBUF = 0xFF;             // Send dummy data to initiate SPI clock
    while (!SSPSTATbits.BF);   // Wait until reception is complete
    return SSPBUF;             // Return received data
    }

void SPI_Set_CS(bool state) {
    if (state) {
        CS_PIN = 1; // Deselect (CS high)
        } else {
        CS_PIN = 0; // Select (CS low)
        }
    }

