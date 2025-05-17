
#include "main.h"
#include "uart.h"
#include "Mpl3115a2.h"

void main(void) {
    // Initialize UART for debugging
    UART_TX_Init();
    
    UART_Write_Text("MPL3115A2 test!\n\r");
    // Initialize the MPL3115A2 sensor
    if (!MPL3115A2_Init()) {
        UART_Write_Text("Could not find sensor. Check wiring.!\n\r");
        while (1); // Infinite loop to halt execution
        }
    
    // Set sea level pressure for accurate altitude measurement
    MPL3115A2_SetSeaPressure(1013.26);
    
    // Main loop
    while (1) {
        // Variables to store sensor data
        float pressure_hpa = MPL3115A2_GetPressure();
        float pressure_pa = pressure_hpa * 100;
        float altitude = MPL3115A2_GetAltitude();
        float temperature = MPL3115A2_GetTemperature();
        
        
        
        UART_Write_Text("pressure = ");
        char pressureText[16];
        sprintf(pressureText, "%.2f hPa\n\r", pressure_pa);
        UART_Write_Text(pressureText);
        
        UART_Write_Text("altitude = ");
        char altitudeText[16];
        sprintf(altitudeText, "%.2f m\n\r", altitude);
        UART_Write_Text(altitudeText);
        
        UART_Write_Text("temperature = ");
        char temperatureText[16];
        sprintf(temperatureText, "%.2f C\n\r", temperature);
        UART_Write_Text(temperatureText);
        
        // Delay before the next reading
        __delay_ms(250);
        }
    }



