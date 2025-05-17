
#include "tc72.h"

void TC72_SetMode(uint8_t mode) {
    SPI_Set_CS(true);                // Deassert CS
    SPI_Write(CONTROL_REG);          // Access Control Register
    SPI_Write(mode);                 // Write the mode to the Control Register
    SPI_Set_CS(false);               // Assert CS
    
    // Allow stabilization if the mode requires it
    if (mode == TC72_MODE_CONTINUOUS || mode == TC72_MODE_ONE_SHOT) {
        __delay_ms(150);             // Allow 150ms for temperature conversion
    }
}

float TC72_ReadTemperature() {
    uint8_t msb, lsb;
    int16_t rawTemperature;
    float temperature = 0.0f;
    
    SPI_Set_CS(true);                // Deassert CS
    SPI_Write(TEMPR_REG);            // Access MSB temperature register
    msb = SPI_Read();                // Read MSB
    lsb = SPI_Read();                // Read LSB
    SPI_Set_CS(false);               // Assert CS
    
    // Combine MSB and LSB to form raw temperature
    rawTemperature = ((int16_t)msb << 8) | lsb;
    
    // Add integer part from MSB
    temperature = (int8_t)(rawTemperature >> 8);
    
    // Extract fractional part from LSB
    if (lsb & 0x40) temperature += 0.25f;  // 0.25C
    if (lsb & 0x80) temperature += 0.5f;   // 0.5C
    
    return temperature;
}

uint8_t TC72_ReadManufacturerID() {
    uint8_t manufacturerID;

    SPI_Set_CS(true);                // Deassert CS
    SPI_Write(CONTROL_REG);          // Access Control Register
    SPI_Write(0x00);                 //
    SPI_Set_CS(false);               // Assert CS

    SPI_Set_CS(true);                // Deassert CS
    SPI_Write(MANUFACTURER_REG);     // Access Manufacturer ID register
    manufacturerID = SPI_Read();     // Read Manufacturer ID
    SPI_Set_CS(false);               // Assert CS

    return manufacturerID;
}


