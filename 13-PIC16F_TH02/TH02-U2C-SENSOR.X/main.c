
#include <xc.h>
#include "i2c.h"
#include "uart.h"
#include "TH02.h"
#include <stdio.h>

void main(void) {
    char displayText[64];
    int16_t tempRaw, humRaw, humComp;
    
    // Initialize I2C and UART
    I2C_Master_Init();
    UART_TX_Init();
    UART_Write_Text("TH02 Sensor Initialization\n\r");
    
    while (1) {
        // Start Temperature Conversion
        UART_Write_Text("Starting Temperature Conversion...\n\r");
        TH02_StartTempConversion(0, 0);
        
        if (TH02_WaitForReady() == 0) {
            tempRaw = TH02_GetConversionValue();
            } else {
            UART_Write_Text("Temperature Conversion Timeout\n\r");
            tempRaw = TH02_UNDEFINED_VALUE;
            }
        
        // Start Humidity Conversion
        UART_Write_Text("Starting Humidity Conversion...\n\r");
        TH02_StartRHConversion(0, 0);
        
        if (TH02_WaitForReady() == 0) {
            humRaw  = TH02_GetConversionValue();
            humComp = TH02_GetCompensatedRH(1);
        } else {
            UART_Write_Text("Humidity Conversion Timeout\n\r");
            humRaw = TH02_UNDEFINED_VALUE;
            humComp = TH02_UNDEFINED_VALUE;
        }
        
        // Format and Send Temperature Data
        if (tempRaw != TH02_UNDEFINED_VALUE) {
            snprintf(displayText, sizeof(displayText), "Temperature: %d.%01d C\n\r", tempRaw / 10, abs(tempRaw % 10));
        } else {
        snprintf(displayText, sizeof(displayText), "Temperature: Error\n\r");
        }
        UART_Write_Text(displayText);

        // Format and Send Raw Humidity Data
        if (humRaw != TH02_UNDEFINED_VALUE) {
        snprintf(displayText, sizeof(displayText), "Humidity (Raw): %d.%01d %%\n\r", humRaw / 10, abs(humRaw % 10));
        } else {
        snprintf(displayText, sizeof(displayText), "Humidity (Raw): Error\n\r");
        }
        UART_Write_Text(displayText);

        // Format and Send Compensated Humidity Data
        if (humComp != TH02_UNDEFINED_VALUE) {
        snprintf(displayText, sizeof(displayText), "Humidity (Compensated): %d.%01d %%\n\r", humComp / 10, abs(humComp % 10));
        } else {
        snprintf(displayText, sizeof(displayText), "Humidity (Compensated): Error\n\r");
        }
        UART_Write_Text(displayText);
        UART_Write_Text("\n\rWaiting 10 seconds for the next measurement...\n\r");
        UART_Write_Text("\n\r************------------------************\n\r");

        __delay_ms(10000); //10 seconds

    }
}