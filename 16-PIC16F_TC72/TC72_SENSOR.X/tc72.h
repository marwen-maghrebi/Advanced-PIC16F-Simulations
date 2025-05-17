
#ifndef TC72_H
#define TC72_H

#include "spi.h"

// TC72 Register Definitions
#define CONTROL_REG       0x80  // Control Register (write access)
#define TEMPR_REG         0x02  // Temperature MSB Register
#define TEMP_LSB_REG      0x01  // Temperature LSB Register
#define MANUFACTURER_REG  0x03  // Manufacturer ID Register

// TC72 Mode Definitions
#define TC72_MODE_CONTINUOUS 0x00  // Continuous Mode
#define TC72_MODE_ONE_SHOT   0x10  // One-Shot Mode
#define TC72_MODE_SHUTDOWN   0x01  // Shutdown Mode

// Function Prototypes
void TC72_SetMode(uint8_t mode);            // Configure the TC72's operational mode
float TC72_ReadTemperature();              // Read the temperature as a float
uint8_t TC72_ReadManufacturerID();         // Read the Manufacturer ID for verification

#endif // TC72_H

