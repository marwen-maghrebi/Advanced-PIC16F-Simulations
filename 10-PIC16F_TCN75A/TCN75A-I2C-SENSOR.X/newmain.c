
#include "TCN75A.h"
#include "i2c.h"
#include "uart.h"
#include <stdio.h>

#define SENSOR_ADDRESS 0x48 // Example I2C address of TCN75A

// Function prototypes
void setupSensorForNormalMode(void);
void setupSensorForSafetyMode(void);
void restartSensor(void);

// Global variables
float actualTemp;
float LimitTemp;
float HystTemp;
char displayText[64];

void setupSensorForNormalMode(void) {
    TCN75A_SetAlertMode(COMP_MODE);
    TCN75A_SetAlertPolarity(0);
    TCN75A_SetRangeTemp(55.0, 60);
    TCN75A_SetFaultQueue(1);
    }

void setupSensorForSafetyMode(void) {
    TCN75A_SetAlertMode(INT_MODE);
    TCN75A_SetAlertPolarity(1);
    TCN75A_SetRangeTemp(65.0, 70);
    TCN75A_SetFaultQueue(2);
    }

void restartSensor(void) {
    TCN75A_SetShutdown(1);
    __delay_ms(500);
    TCN75A_SetShutdown(0);
    }

int main(void) {
    // Initialize I2C, UART, and TCN75A
    I2C_Master_Init();
    UART_TX_Init();
    TCN75A_Init(SENSOR_ADDRESS);
    UART_Write_Text("TCN75A initialized");
       
    // Main loop
    while (1) {
        // Read the current temperature
        actualTemp = TCN75A_ReadTemperature();
        sprintf(displayText, "Temp: %.1f C\n\r", actualTemp);
        UART_Write_Text(displayText);
        // Switch based on button states
        if (RB0 == 1) { // Default Mode
            UART_Write_Text("Default Modex\n\r");
            // Get default temperature thresholds
            sprintf(displayText, "Default LimitTemp:%.1fC, Hysteresis:%.1fC\n\r", TCN75A_GetLimitTemp(), TCN75A_GetHystTemp());
            UART_Write_Text(displayText);
            if (actualTemp > TCN75A_GetLimitTemp()) {
                UART_Write_Text("You crossed the Default LimitTemp\n\r");
                }
            }
        else if (RB1 == 1) { // Normal Mode
            UART_Write_Text("Normal Mode\n\r");
            restartSensor();
            setupSensorForNormalMode();
            sprintf(displayText,"Normal mode Thresholds: LimitTemp:%.1f C,Hysteresis: %.1f C\n\r" ,TCN75A_GetLimitTemp(), TCN75A_GetHystTemp());
            UART_Write_Text(displayText);
            if (actualTemp > TCN75A_GetLimitTemp()) {
                UART_Write_Text("You crossed the NormalMode LimitTemp\n\r");
                }
            }
        else if (RB2 == 1) { // Safety Mode
            UART_Write_Text("Safety Mode");
            restartSensor();
            setupSensorForSafetyMode();
            sprintf(displayText,"Safety mode Thresholds: LimitTemp: %.1f C,Hysteresis: %.1fC\n\r" ,TCN75A_GetLimitTemp(), TCN75A_GetHystTemp());
            UART_Write_Text(displayText);
            if (actualTemp > TCN75A_GetLimitTemp()) {
                UART_Write_Text("You crossed the SafetyMode LimitTemp\n\r");
                }
            }
        // Delay before checking again
        __delay_ms(1000);
        }
    return 0;
    }

