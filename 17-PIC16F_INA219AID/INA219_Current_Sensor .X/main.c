
#include "main.h"
#include "INA219.h"
#include "uart.h"

void main(void) {
    
    // Variables to hold sensor values
    float shuntVoltage = 0.0;
    float busVoltage = 0.0;
    float current_mA = 0.0;
    float loadVoltage = 0.0;
    float power_mW = 0.0;
    
    char buffer[64];
    // Initialization
    UART_TX_Init();     // Initialize UART for debugging
    INA219_Init();      // Initialize INA219 sensor
    
    UART_Write_Text("Measuring voltage and current with INA219 ... \n\r");
    
    
    
    while (1) {
        // Read INA219 sensor values
        shuntVoltage = INA219_GetShuntVoltage_mV();
        busVoltage   = INA219_GetBusVoltage_V();
        current_mA   = INA219_GetCurrent_mA();
        power_mW     = INA219_GetPower_mW();
        loadVoltage  = busVoltage + (shuntVoltage / 1000.0);
        
        // Format and transmit readings over UART
        sprintf(buffer, "Bus Voltage:   %.2f V\n\r", busVoltage);
        UART_Write_Text(buffer);
        
        sprintf(buffer, "Shunt Voltage: %.2f mV\n\r", shuntVoltage);
        UART_Write_Text(buffer);
        
        sprintf(buffer, "Load Voltage:  %.2f V\n\r", loadVoltage);
        UART_Write_Text(buffer);
        
        sprintf(buffer, "Current:       %.2f mA\n\r", current_mA);
        UART_Write_Text(buffer);
        
        sprintf(buffer, "Power:         %.2f mW\n\r", power_mW);
        UART_Write_Text(buffer);
        
        UART_Write_Text("-----------------------------\n\r");
        
        __delay_ms(2000); // Delay for 2 seconds
        }
    }

