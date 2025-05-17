#include "SHT2x.h"

// Helper function to read data from the sensor
static uint16_t SHT2x_ReadSensor(uint8_t command) {
    uint16_t result = 0;
    
    // Start I2C communication and send command
    I2C_Master_Start();
    I2C_Master_Write((SHT2x_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(command);
    I2C_Master_Stop();
    
    __delay_ms(100);  // Sensor processing time
    
    // Request data from sensor
    I2C_Master_Start();
    I2C_Master_Write((SHT2x_ADDRESS << 1) | 1); // Read mode
    
    // Read two bytes and ignore status bits
    result = I2C_Read_Byte() << 8;
    result |= I2C_Read_Byte();
    result &= ~0x0003; // Clear status bits
    
    I2C_NACK(); // Send NACK to stop communication
    I2C_Master_Stop();
    
    return result;
}

// GetHumidity: Returns the humidity value in %RH
float SHT2x_GetHumidity(void) {
    float value = (float) SHT2x_ReadSensor(RH_HOLD_CMD);
    if (value == 0) {
        return 0; // Indicate error with an unrealistic value
        }
    return -6.0 + 125.0 * value / 65536.0;
}

// GetTemperature: Returns the temperature in degrees Celsius
float SHT2x_GetTemperature(void) {
    float value = (float) SHT2x_ReadSensor(TEMP_HOLD_CMD);
    if (value == 0) {
        return -273; // Indicate error with an unrealistic value (absolute zero)
        }
    return -46.85 + 175.72 * value / 65536.0;
}

// GetDewPoint: Calculates and returns the dew point based on humidity and temperature
float SHT2x_GetDewPoint(void) {
    float humidity = SHT2x_GetHumidity();
    float temperature = SHT2x_GetTemperature();
    
    // Check for error conditions in temperature or humidity
    if (temperature == -273 || humidity == 0) {
        return -273; // Indicate error in dew point calculation
        }
    
    // Calculate intermediate gamma value
    float gamma = log(humidity / 100.0) + WATER_VAPOR * temperature / (BAROMETRIC_PRESSURE + temperature);
    // Calculate dew point
    return BAROMETRIC_PRESSURE * gamma / (WATER_VAPOR - gamma);
}

