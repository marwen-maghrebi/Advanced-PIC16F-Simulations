
#include <xc.h>
#include "I2C.h"
#include "SHT2x.h"
#include "HYT-271.h"
#include "jhd_lcd.h"
#include <stdio.h>

// LCD Address
#define LCD_ADDRESS 0x7C

// Create a buffer for formatted display text
char displayText[17];

void main(void) {
    // Initialize I2C and HYT271 sensor
    HYT271_Init();

    // Initialize LCD
    LCD_Init(LCD_ADDRESS);
    LCD_Clear();

    while(1) {
        // Start sensor measurement for HYT271
        HYT271_StartMeasurement();

        // Get the processed temperature and humidity values from HYT271
        float temperatureHYT = HYT271_GetTemperature();
        float humidityHYT = HYT271_GetHumidity();

        // Read Temperature and Humidity from SHT21 sensor
        float SHT21_temperature = SHT2x_GetTemperature();
        float SHT21_humidity    = SHT2x_GetHumidity();

        // Display the information from both sensors
        LCD_SetCursor(0, 0);  // Set cursor to first row

        // Format the string for displaying on the LCD
        sprintf(displayText, "HYT:%.1fC %.1f%%", temperatureHYT, humidityHYT);
        LCD_Print_String(displayText);  // Print the formatted string for HYT-271

        // Display HTU21D data on the second row
        LCD_SetCursor(0, 1);  // Set cursor to second row
        sprintf(displayText, "SHT:%.1fC %.1f%%", SHT21_temperature, SHT21_humidity);
        LCD_Print_String(displayText);  // Print the formatted string for HTU-21D

        // Wait before next reading
        __delay_ms(1000);
    }
}
