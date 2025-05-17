
#include <xc.h>
#include "i2c.h"
#include "HYT-271.h"

// Global variables to hold temperature and humidity
static float endValueTemp = 0.0;
static float endValueCap = 0.0;

void HYT271_Init(void) {
    // Initialize I2C for communication
    I2C_Master_Init();
    }

void HYT271_StartMeasurement(void) {
    unsigned char data[4];
    unsigned int valueTempH, valueTempL, valueTemp;
    unsigned int valueCapH, valueCapL, valueCap;
    
    // Start the sensor measurement
    I2C_Master_Start();
    I2C_Master_Write(SLAVE_ADDR << 1);  // Write mode
    I2C_Master_Stop();
    
    // Wait for the sensor to complete measurement
    __delay_ms(50);
    
    // Read sensor data (4 bytes)
    I2C_Master_Start();
    I2C_Master_Write((SLAVE_ADDR << 1) | 1);  // Read mode
    
    // Read 4 bytes with proper ACK/NACK
    data[0] = I2C_Read_Byte();
    I2C_ACK();
    
    data[1] = I2C_Read_Byte();
    I2C_ACK();
    
    data[2] = I2C_Read_Byte();
    I2C_ACK();
    
    data[3] = I2C_Read_Byte();
    I2C_NACK();
    
    I2C_Master_Stop();
    
    // Process the sensor data
    valueCapH = data[0] & 0x3F;  // Mask status bits for humidity
    valueCapL = data[1];
    valueTempH = data[2];        // No mask for temperature high byte
    valueTempL = data[3] & 0xFC; // Mask the lower 2 bits of temperature
    
    // Calculate the raw humidity and temperature values
    valueCap = ((unsigned int)valueCapH << 8) | valueCapL;
    valueTemp = ((unsigned int)valueTempH << 6) | (valueTempL >> 2);
    
    // Convert to actual values (temperature and humidity)
    endValueCap = ((float)valueCap / 16383.0) * 100.0;
    endValueTemp = ((float)valueTemp / 16383.0) * 165.0 - 40.0;
    }

float HYT271_GetTemperature(void) {
    return endValueTemp;
    }

float HYT271_GetHumidity(void) {
    return endValueCap;
    }


