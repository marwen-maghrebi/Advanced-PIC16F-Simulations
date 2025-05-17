
#include "main.h"
#include "adc.h"
#include "lcd.h"

// Message buffers
char message1[16];
char message2[16];

// Main function
void main(void) {
    // Initialize peripherals
    lcd_initialize();
    configure_ADC();
    
    // Define constants for MCP9700 and TC1047
    const float VREF = 3.3;           // ADC reference voltage (3.3V)
    const int ADC_RESOLUTION = 1024; // 10-bit ADC resolution
    const float MCP9700_TEMP_OFFSET = 0.5;   // MCP9700 offset at 0C (500 mV)
    const float MCP9700_TEMP_SLOPE = 0.01;   // MCP9700 slope (10 mV/C)
    
    while (1) {
        // --- MCP9700 Sensor Reading ---
        select_ADC_channel(0); // Select AN0 for MCP9700 sensor
        start_ADC_conversion();
        wait_for_conversion();
        int MCP9700_ADCValue = read_ADC_result();
        
        // Convert MCP9700 ADC value to voltage
        float MCP9700_voltage = ((float)MCP9700_ADCValue * VREF) / ADC_RESOLUTION;
        
        // Convert MCP9700 voltage to temperature (Celsius)
        float MCP9700_temperature = (MCP9700_voltage - MCP9700_TEMP_OFFSET) / MCP9700_TEMP_SLOPE;
        
        // Format and display MCP9700 temperature
        sprintf(message1, "MCP9700:%.2f C", MCP9700_temperature);
        lcd_command(0x80); // Move to first line
        lcd_string((const unsigned char *)message1, sizeof(message1));
        
        // --- TC1047 Sensor Reading ---
        select_ADC_channel(1); // Select AN1 for TC1047 sensor
        start_ADC_conversion();
        wait_for_conversion();
        int TC1047_ADCValue = read_ADC_result();
        
        // Convert TC1047 ADC value to voltage in millivolts
        float TC1047_voltage_mV = ((float)TC1047_ADCValue * VREF * 1000.0) / ADC_RESOLUTION;
        
        // Convert TC1047 voltage to temperature (Celsius)
        float TC1047_temperature = (TC1047_voltage_mV - 500.0) / 10.0;
        
        // Format and display TC1047 temperature
        sprintf(message2, "TC1047 :%.2f C", TC1047_temperature);
        lcd_command(0xC0); // Move to second line
        lcd_string((const unsigned char *)message2, sizeof(message2));
        }
    }


