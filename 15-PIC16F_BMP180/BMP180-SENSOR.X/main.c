
#include "i2c.h"
#include "BMP085.h"
#include "lcd.h"


void main(void) {
    // Initialize LCD and BMP085
    lcd_initialize();
    BMP085_Init(3); // Ultra High-Res Mode
    
    
    
    while (1) {
        char buffer[16];
        // Format and display MCP9700 temperature
        /*float temperature = BMP085_ReadTemperature();
        sprintf(buffer, "Temp = %.2f*C", temperature);
        lcd_command(0x80); // Move to first line
        lcd_string((const unsigned char *)buffer, sizeof(buffer));
        */
        int32_t pressure = (BMP085_ReadPressure()/100); // /100 to convert to hpa
        sprintf(buffer, "Pres = %ldhPa", pressure);
        lcd_command(0xC0); // Move to second line
        lcd_string((const unsigned char *)buffer, sizeof(buffer));
        __delay_ms(500);
        }
    }


