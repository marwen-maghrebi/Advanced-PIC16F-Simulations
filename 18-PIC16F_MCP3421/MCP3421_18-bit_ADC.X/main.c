
#include "main.h"
#include "uart.h"
#include "i2c.h"
#include "MCP3421.h"

void main(void) {
    // Initialize UART and I2C modules
    UART_TX_Init();
    I2C_Master_Init();
    
    // Initialize MCP3421 ADC
    MCP3421 adc;
    MCP3421_Init(&adc, 0x68, 2, 0); // Address 0x68, 16-bit, PGA x1
    
    // Main loop
    while (1) {
        // Check if MCP3421 is ready for a new conversion
        if (MCP3421_Ready(&adc)) {
            // Get the voltage reading
            double voltage = MCP3421_GetDouble(&adc);
            double millivolts = voltage*1000;
            // Convert voltage to string and send via UART
            char buffer[20];
            sprintf(buffer, "Voltage: %.2f mV\n\r", millivolts);
            UART_Write_Text(buffer);
            
            // Delay before next reading
            __delay_ms(1000);
            }
        }
    }


  