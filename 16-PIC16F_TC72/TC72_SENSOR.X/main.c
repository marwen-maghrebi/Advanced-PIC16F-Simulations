
#include "main.h"
#include "uart.h"
#include "spi.h"
#include "tc72.h"

void main(void) {
    // Initialize UART and SPI
    UART_TX_Init();
    SPI_Master_Init();
    
    // Verify Manufacturer ID
    uint8_t manufacturerID = TC72_ReadManufacturerID();
    UART_Write_Text("Manufacturer ID: 0x");
    char buffer[5];
    sprintf(buffer, "%02X", manufacturerID);
    UART_Write_Text(buffer);
    UART_Write_Text("\n\r");
    
    if (manufacturerID != 0x54) {
        UART_Write_Text("Error: Incorrect Manufacturer ID\n\r");
        while (1); // Halt the program if the sensor is not detected
        }
    
    // Initialize TC72 in Continuous Mode
    TC72_SetMode(TC72_MODE_CONTINUOUS);
    UART_Write_Text("TC72 Initialized in Continuous Mode\n\r");
    
    while (1) {
        // Read temperature
        float temperature = TC72_ReadTemperature();
        
        // Print temperature
        UART_Write_Text("Temperature: ");
        sprintf(buffer, "%.2f C", temperature);
        UART_Write_Text(buffer);
        UART_Write_Text("\n\r");
        
        // Simulate low-power use case by switching to One-Shot Mode
        __delay_ms(1000); // Wait for 1 second
        UART_Write_Text("Switching to One-Shot Mode...\n\r");
        TC72_SetMode(TC72_MODE_ONE_SHOT);
        __delay_ms(150); // Wait for single conversion
        temperature = TC72_ReadTemperature();
        UART_Write_Text("One-Shot Temperature: ");
        sprintf(buffer, "%.2f C", temperature);
        UART_Write_Text(buffer);
        UART_Write_Text("\n\r");
        
        // Switch back to Shutdown Mode to save power
        UART_Write_Text("Switching to Shutdown Mode...\n\r");
        TC72_SetMode(TC72_MODE_SHUTDOWN);
        __delay_ms(1000); // Simulate idle time
        }
    }


