
#include "BMP085.h"


uint8_t BMP085_Read8(uint8_t addr) {
    uint8_t data;
    
    I2C_Master_Start();                // Start condition
    I2C_Master_Write(BMP085_I2CADDR << 1);  // Send I2C address with write bit
    I2C_Master_Write(addr);            // Send the register address
    I2C_Master_RepeatedStart();        // Send a repeated start
    I2C_Master_Write((BMP085_I2CADDR << 1) | 1); // Send I2C address with read bit
    data = I2C_Read_Byte();            // Read the data byte
    I2C_NACK();                        // Send NACK to end the read
    I2C_Master_Stop();                 // Stop condition
    
    return data;                       // Return the received data
    }

uint16_t BMP085_Read16(uint8_t addr) {
    uint16_t data;
    
    I2C_Master_Start();                // Start condition
    I2C_Master_Write(BMP085_I2CADDR << 1);  // Send I2C address with write bit
    I2C_Master_Write(addr);            // Send the register address
    I2C_Master_RepeatedStart();        // Send a repeated start
    I2C_Master_Write((BMP085_I2CADDR << 1) | 1); // Send I2C address with read bit
    data = ((uint16_t)I2C_Read_Byte() << 8); // Read MSB
    I2C_ACK();                         // Send ACK to continue the read
    data |= I2C_Read_Byte();           // Read LSB
    I2C_NACK();                        // Send NACK to end the read
    I2C_Master_Stop();                 // Stop condition
    
    return data;                       // Return the combined 16-bit data
    }

inline void BMP085_Write8(uint8_t addr, uint8_t data) {
    I2C_Master_Start();                // Start condition
    I2C_Master_Write(BMP085_I2CADDR << 1);  // Send I2C address with write bit
    I2C_Master_Write(addr);            // Send the register address
    I2C_Master_Write(data);            // Send the data
    I2C_Master_Stop();                 // Stop condition
    }

void BMP085_Init(uint8_t mode) {
    I2C_Master_Init();
    
    if (mode > 3) mode = 3;
    oversampling = mode;
    
    // Read calibration data from sensor
    ac1 = (int16_t)BMP085_Read16(BMP085_CAL_AC1);
    ac2 = (int16_t)BMP085_Read16(BMP085_CAL_AC2);
    ac3 = (int16_t)BMP085_Read16(BMP085_CAL_AC3);
    ac4 = (uint16_t)BMP085_Read16(BMP085_CAL_AC4);
    ac5 = (uint16_t)BMP085_Read16(BMP085_CAL_AC5);
    ac6 = (uint16_t)BMP085_Read16(BMP085_CAL_AC6);
    
    b1 = (int16_t)BMP085_Read16(BMP085_CAL_B1);
    b2 = (int16_t)BMP085_Read16(BMP085_CAL_B2);
    
    mb = (int16_t)BMP085_Read16(BMP085_CAL_MB);
    mc = (int16_t)BMP085_Read16(BMP085_CAL_MC);
    md = (int16_t)BMP085_Read16(BMP085_CAL_MD);
    }

int32_t BMP085_ComputeB5(int32_t UT){
    int32_t X1 = (UT - (int32_t)ac6) * ((int32_t)ac5) >> 15;
    int32_t X2 = ((int32_t)mc << 11) / (X1 + (int32_t)md);
    return X1 + X2;
    }

float BMP085_ReadTemperature(void) {
    int32_t UT, B5;
    float temp;
    
    BMP085_Write8(BMP085_CONTROL, BMP085_READTEMPCMD);
    __delay_ms(5);
    UT = BMP085_Read16(BMP085_TEMPDATA);
    
    B5 = BMP085_ComputeB5(UT);
    temp = (B5 + 8) >> 4;
    return (float)(temp / 10.0);
    }


uint16_t BMP085_ReadRawTemperature(void) {
    BMP085_Write8(BMP085_CONTROL, BMP085_READTEMPCMD); // Write command to control register
    __delay_ms(5);                                    // Delay for conversion (minimum 4.5ms)
    return BMP085_Read16(BMP085_TEMPDATA);            // Read and return the 16-bit raw temperature data
    }

uint32_t BMP085_ReadRawPressure(void) {
    uint32_t raw;
    
    BMP085_Write8(BMP085_CONTROL, BMP085_READPRESSURECMD + (oversampling << 6)); // Write command with oversampling setting
    
    // Delay based on oversampling mode
    if (oversampling == 0)         // Ultra Low Power
        __delay_ms(5);
    else if (oversampling == 1)    // Standard
        __delay_ms(8);
    else if (oversampling == 2)    // High Resolution
        __delay_ms(14);
    else                           // Ultra High Resolution
        __delay_ms(26);
    
    // Read the raw pressure data
    raw = BMP085_Read16(BMP085_PRESSUREDATA);         // Read the MSB and LSB
    raw <<= 8;                                        // Shift MSB to its correct position
    raw |= BMP085_Read8(BMP085_PRESSUREDATA + 2);     // Read and append the XLSB
    raw >>= (8 - oversampling);                       // Adjust based on oversampling setting
    
    return raw;                                       // Return the 19-bit raw pressure data
    }

int32_t BMP085_ReadPressure(void) {
    int32_t UT, UP, B3, B5, B6, X1, X2, X3, p;
    uint32_t B4, B7;
    
    UT = BMP085_ReadRawTemperature();
    UP = BMP085_ReadRawPressure();
    
    B5 = BMP085_ComputeB5(UT);
    B6 = B5 - 4000;
    X1 = (b2 * ((B6 * B6) >> 12)) >> 11;
    X2 = (ac2 * B6) >> 11;
    X3 = X1 + X2;
    B3 = (((ac1 * 4 + X3) << oversampling) + 2) / 4;
    
    X1 = (ac3 * B6) >> 13;
    X2 = (b1 * ((B6 * B6) >> 12)) >> 16;
    X3 = ((X1 + X2) + 2) >> 2;
    B4 = (ac4 * (uint32_t)(X3 + 32768)) >> 15;
    B7 = ((uint32_t)UP - B3) * (50000U >> oversampling);
    
    if (B7 < 0x80000000U) {
        p = (int32_t)((B7 * 2) / B4);
        } else {
        p = (int32_t)((B7 / B4) * 2);
        }
    
    X1 = (p >> 8) * (p >> 8);
    X1 = (X1 * 3038) >> 16;
    X2 = (-7357 * p) >> 16;
    p = p + ((X1 + X2 + 3791) >> 4);
    
    return p;
    }
