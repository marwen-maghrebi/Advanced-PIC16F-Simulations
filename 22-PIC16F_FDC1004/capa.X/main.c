
#include "i2c.h"
#include "lcd.h"
#include "FDC1004.h"

#define UPPER_BOUND  0x4000
#define LOWER_BOUND  (-1 * UPPER_BOUND)
#define CHANNEL_1 0
#define MEASUREMENT_1 0

#define CHANNEL_2 1
#define MEASUREMENT_2 1

void main(void) {
    FDC1004 fdc;
    uint16_t value_1[2], value_2[2];
    int capdac_1 = 0, capdac_2 = 0;
    
    // Initialize I2C and LCD
    I2C_Master_Init();
    lcd_initialize();
    FDC1004_Init(&fdc, FDC1004_400HZ);
    
    while (1) {
        // Configure and trigger a single measurement for Channel 1
        FDC1004_ConfigureMeasurement(&fdc, MEASUREMENT_1, CHANNEL_1, capdac_1);
        FDC1004_TriggerMeasurement(&fdc, MEASUREMENT_1, FDC1004_100HZ);
        
        if (!FDC1004_ReadMeasurement(&fdc, MEASUREMENT_1, value_1)) {
            int16_t msb_1 = (int16_t)value_1[0];
            int32_t capacitance_1 = ((int32_t)457) * ((int32_t)msb_1); // in attofarads
            capacitance_1 /= 1000; // in femtofarads
            capacitance_1 += ((int32_t)3028) * ((int32_t)capdac_1);
            
            // Display capacitance for Channel 1 on the LCD
            char buffer_1[16];
            sprintf(buffer_1, "C1=%.4f pF", (float)capacitance_1 / 1000);
            lcd_command(0x80); // Move to the first line
            lcd_string((const unsigned char *)buffer_1, sizeof(buffer_1));
            
            // Adjust capdac if necessary for Channel 1
            if (msb_1 > UPPER_BOUND) {
                if (capdac_1 < FDC1004_CAPDAC_MAX)
                    capdac_1++;
                } else if (msb_1 < LOWER_BOUND) {
                    if (capdac_1 > 0)
                        capdac_1--;
                    }
            }
        }
    }




