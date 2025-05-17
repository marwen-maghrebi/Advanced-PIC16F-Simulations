
#include "TH02.h"
#include <math.h>

// Global state variables
int32_t _last_temp = TH02_UNINITIALIZED_TEMP;
int32_t _last_rh   = TH02_UNINITIALIZED_RH;
uint8_t last_conversion_type = 0; // 1 for temp, 0 for RH

uint8_t TH02_WriteRegister(uint8_t reg, uint8_t value) {
    I2C_Master_Start();
    if (I2C_Master_Write(TH02_I2C_ADDR << 1) != 0) return 1;
    if (I2C_Master_Write(reg) != 0) return 1;
    if (I2C_Master_Write(value) != 0) return 1;
    I2C_Master_Stop();
    return 0;
    }

uint8_t TH02_ReadRegister(uint8_t reg, uint8_t *value) {
    I2C_Master_Start();
    if (I2C_Master_Write(TH02_I2C_ADDR << 1) != 0) return 1;
    if (I2C_Master_Write(reg) != 0) return 1;
    
    I2C_Master_RepeatedStart();
    if (I2C_Master_Write((TH02_I2C_ADDR << 1) | 0x01) != 0) return 1;
    
    *value = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    return 0;
    }

uint8_t TH02_StartTempConversion(uint8_t fastmode, uint8_t heater) {
    uint8_t config = TH02_CONFIG_START | TH02_CONFIG_TEMP;
    if (fastmode) config |= TH02_CONFIG_FAST;
    if (heater)   config |= TH02_CONFIG_HEAT;   
    return TH02_WriteRegister(TH02_CONFIG, config);
    }

uint8_t TH02_StartRHConversion(uint8_t fastmode, uint8_t heater) {
    uint8_t config = TH02_CONFIG_START;
    if (fastmode) config |= TH02_CONFIG_FAST;
    if (heater)   config |= TH02_CONFIG_HEAT;
    return TH02_WriteRegister(TH02_CONFIG, config);
    }

uint8_t TH02_WaitForReady(void) {
    uint8_t status;
    uint8_t time_out = 0;
    
    // Loop until conversion is done or timeout occurs
    while (time_out <= TH02_CONVERSION_TIME_OUT) {
        if (TH02_ReadRegister(TH02_STATUS, &status) == 0) {
            // Check if RDY bit is cleared (conversion complete)
            if ((status & TH02_STATUS_RDY) == 0) {
                return 0; // Conversion complete
                }
            }
        // Increment timeout and delay for 1 millisecond
        time_out++;
        __delay_ms(1);
        }
    
    return 1; // Timeout occurred
    }

int16_t roundInt(float value) {
    if (value >= 0.0f) {
        value = floor(value + 0.5f);
        } else {
        value = ceil(value - 0.5f);
        }
    return (int16_t)value;
    }

int16_t TH02_GetCompensatedRH(uint8_t round_flag) {
    float rhvalue, rhlinear;
    int16_t ret = TH02_UNDEFINED_VALUE;
    
    if (_last_rh != TH02_UNINITIALIZED_RH) {
        rhvalue = (float)_last_rh / 100.0f;
        
        rhlinear = rhvalue - ((rhvalue * rhvalue) * TH02_A2 +
                rhvalue * TH02_A1 +
                TH02_A0);
        
        if (_last_temp != TH02_UNINITIALIZED_TEMP) {
            rhlinear += ((_last_temp / 100.0f) - 30.0f) *
                    (rhlinear * TH02_Q1 + TH02_Q0);
            }
        
        rhvalue = rhlinear * 100.0f;
        
        if (round_flag) {
            ret = (int16_t)((rhvalue + 5.0f) / 10.0f);
            } else {
            ret = (int16_t)rhvalue;
            }
        }
    
    return ret;
    }

int16_t TH02_GetConversionValue(void) {
    uint16_t msb = 0, lsb = 0;
    int32_t result = 0;
    int16_t ret = TH02_UNDEFINED_VALUE;
    uint8_t config;
    
    // Start I2C communication
    I2C_Master_Start();
    if (I2C_Master_Write((TH02_I2C_ADDR << 1)) != 0) {
        I2C_Master_Stop();
        return ret;
        }
    
    // Send data register address
    if (I2C_Master_Write(TH02_DATAh) != 0) {
        I2C_Master_Stop();
        return ret;
        }
    
    // Send a repeated start condition
    I2C_Master_RepeatedStart();
    if (I2C_Master_Write((TH02_I2C_ADDR << 1) | 0x01) != 0) {
        I2C_Master_Stop();
        return ret;
        }
    
    // Read MSB and LSB of the data
    msb = I2C_Read_Byte();
    I2C_ACK();
    lsb = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    
    // Combine MSB and LSB into a 16-bit result
    result = ((uint16_t)msb << 8) | lsb;
    
    // Read the configuration to determine the last conversion type
    if (TH02_ReadRegister(TH02_CONFIG, &config) == 0) {
        if (config & TH02_CONFIG_TEMP) {
            // Temperature result processing
            result >>= 2;        // Remove 2 unused LSB bits
            result *= 100;       // Multiply by 100 for decimal scaling
            result /= 32;        // Apply datasheet formula
            result -= 5000;      // Adjust for 0C offset
            _last_temp = result; // Store last temperature value
            } else {
            // Humidity result processing
            result >>= 4;        // Remove 4 unused LSB bits
            result *= 100;       // Multiply by 100 for decimal scaling
            result /= 16;        // Apply datasheet formula
            result -= 2400;      // Adjust for humidity offset
            _last_rh = result;   // Store last humidity value
            }
        
        // Convert result to 1 decimal place, avoid floats
        ret = roundInt(result / 10.0f);
        }
    
    return ret;
    }
