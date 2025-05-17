#include "i2c.h"
#include "uart.h"
#include "LM75.h"
#include <stdio.h>

int main(void) {
    char buffer[64];
    // Initialize LM75 and peripherals
    LM75_init();
    UART_TX_Init();
    
    float currentTemp,hysteresisTemp,osTripTemp;
    
    
    UART_Write_Text("LM75 Initialized\n\r");
    
    while (1) {
        // Read the current temperature
        currentTemp = LM75_read(TEMPERATURE_ADDRESS);
        sprintf(buffer, "Temp: %.1f C\n\r", currentTemp);
        UART_Write_Text(buffer);
        if (RB0 == 1) { // Default Mode
            UART_Write_Text("Default Mode\n\r");
            hysteresisTemp = LM75_getHysteresisTemperature();
            osTripTemp     = LM75_getOSTripTemperature();
            sprintf(buffer, "Default Thresholds:\nOS Trip: %.1fC\nHysteresis: %.1fC\n\r", osTripTemp, hysteresisTemp);
            UART_Write_Text(buffer);
            
            if (currentTemp > osTripTemp) {
                UART_Write_Text("Warning: Temperature exceeded default threshold\n\r");
                }
            }
        else if (RB1 == 1) { // Normal Mode
            UART_Write_Text("Normal Mode\n\r");
            // Configure LM75 for normal mode
            LM75_setDeviceMode(0); // Comparator mode
            LM75_setOSPolarity(0); // Active low O.S. output
            LM75_setOSTripTemperature(60.0); // Trigger over-temp at 60C
            LM75_setHysteresisTemperature(55.0); // Clear over-temp at 55C
            LM75_setFaultQueue(1); // Require 1 fault
            hysteresisTemp = LM75_getHysteresisTemperature();
            osTripTemp     = LM75_getOSTripTemperature();
            sprintf(buffer, "Normal Mode Thresholds:\nOS Trip: %.1fC\nHysteresis: %.1fC\n\r", osTripTemp, hysteresisTemp);
            UART_Write_Text(buffer);
            if (currentTemp > osTripTemp) {
                UART_Write_Text("Warning: Temperature exceeded Normal mode threshold\n\r");
                }
            }
        else if (RB2 == 1) { // Safety Mode
            UART_Write_Text("Safety Mode"
                    ""
                    "");
            LM75_setDeviceMode(1); // Interrupt mode
            LM75_setOSPolarity(1); // Active low O.S. output
            LM75_setOSTripTemperature(70.0); // Trigger over-temp at 70C
            LM75_setHysteresisTemperature(65.0); // Clear over-temp at 65C
            LM75_setFaultQueue(2); // Require 2 consecutive faults
            hysteresisTemp = LM75_getHysteresisTemperature();
            osTripTemp     = LM75_getOSTripTemperature();
            sprintf(buffer, "Safety Mode Thresholds:\n OS Trip: %.1fC \nHysteresis: %.1fC\n\r", osTripTemp, hysteresisTemp);
            UART_Write_Text(buffer);
            if (currentTemp > osTripTemp) {
                UART_Write_Text("Warning: Temperature exceeded Safety mode threshold\n\r");
                }
            }
        __delay_ms(1000);  // Delay before checking again
        }
    return 0;
    }
