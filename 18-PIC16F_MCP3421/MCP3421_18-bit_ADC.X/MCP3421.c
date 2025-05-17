
#include "MCP3421.h"

// Initialize MCP3421 Configuration
void MCP3421_Init(MCP3421 *adc, int address, uint8_t sr, uint8_t pga) {
    adc->address = address;
    adc->sr = sr & 0x03;  // Sample Rate limited to 2 bits
    adc->pga = pga & 0x03; // PGA Gain limited to 2 bits
    
    adc->config = (adc->sr << 2) | adc->pga;
    adc->config |= (1 << MCP3421_RDY_BIT);  // RDY bit
    adc->config |= (1 << MCP3421_OC_BIT);   // One-shot conversion
    
    I2C_Master_Start();
    I2C_Master_Write(adc->address << 1); // Write mode
    I2C_Master_Write(adc->config);
    I2C_Master_Stop();
    }

// Get Raw Long Data from MCP3421
long MCP3421_GetLong(MCP3421 *adc) {
    uint8_t b2, b3, b4, confRead;
    long result = 0;
    
    I2C_Master_Start();
    I2C_Master_Write((adc->address << 1) | 1); // Read mode
    
    if (adc->sr < 3) {
        // 12, 14, or 16 bits
        b2 = I2C_Read_Byte();
        I2C_ACK();
        b3 = I2C_Read_Byte();
        I2C_ACK();
        confRead = I2C_Read_Byte();
        I2C_NACK();
        result = (b2 << 8) | b3;
        } else {
        // 18 bits
        b2 = I2C_Read_Byte();
        I2C_ACK();
        b3 = I2C_Read_Byte();
        I2C_ACK();
        b4 = I2C_Read_Byte();
        I2C_ACK();
        confRead = I2C_Read_Byte();
        I2C_NACK();
        result = ((long)b2 << 16) | (b3 << 8) | b4;
        if (b2 & 0x10) result |= 0xFF000000; // Sign extend for negative values
        }
    
    I2C_Master_Stop();
    return result;
    }

// Get Voltage as Double from MCP3421
double MCP3421_GetDouble(MCP3421 *adc) {
    long raw = MCP3421_GetLong(adc);
    double voltage = raw * 1e-3 / (1 << (adc->sr * 2)); // Adjust for sample rate
    voltage /= (1 << adc->pga); // Adjust for PGA gain
    return voltage;
    }

// Check if MCP3421 is Ready for a New Conversion
int MCP3421_Ready(MCP3421 *adc) {
    uint8_t confRead;
    I2C_Master_Start();
    I2C_Master_Write((adc->address << 1) | 1); // Read mode
    confRead = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    return !(confRead & (1 << MCP3421_RDY_BIT));
    }

    
