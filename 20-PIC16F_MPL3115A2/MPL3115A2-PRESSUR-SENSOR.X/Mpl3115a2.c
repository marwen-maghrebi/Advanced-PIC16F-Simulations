#include "Mpl3115a2.h"

// Private variables
mpl3115a2_mode_t currentMode;
ctrl_reg1 _ctrl_reg1;

bool MPL3115A2_Init(void) {
    I2C_Master_Init(); // Initialize the I2C master
    // Verify sensor ID
    uint8_t whoami = MPL3115A2_ReadRegister(MPL3115A2_WHOAMI);
    if (whoami != 0xC4) {
        // Sensor ID mismatch
        return false;
    }
    // Software reset
    MPL3115A2_WriteRegister(MPL3115A2_CTRL_REG1, MPL3115A2_CTRL_REG1_RST);
    while (MPL3115A2_ReadRegister(MPL3115A2_CTRL_REG1) & MPL3115A2_CTRL_REG1_RST) {
        __delay_ms(10); // Wait for reset to complete
    }
    // Set oversampling and altitude mode
    _ctrl_reg1.reg = MPL3115A2_CTRL_REG1_OS128 | MPL3115A2_CTRL_REG1_ALT;
    MPL3115A2_WriteRegister(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);
    currentMode = MPL3115A2_ALTIMETER;

    // Enable data ready events for pressure/altitude and temperature
    MPL3115A2_WriteRegister(MPL3115A2_PT_DATA_CFG, MPL3115A2_PT_DATA_CFG_TDEFE |
                            MPL3115A2_PT_DATA_CFG_PDEFE | MPL3115A2_PT_DATA_CFG_DREM);

    return true;
}

float MPL3115A2_GetLastConversionResults(mpl3115a2_meas_t value) {
    uint8_t buffer[5];
    uint32_t pressure;
    int32_t altitude;
    int16_t temperature;

    // Read data from sensor
    I2C_Master_Start();
    I2C_Master_Write((MPL3115A2_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(MPL3115A2_REGISTER_PRESSURE_MSB);
    I2C_Master_RepeatedStart();
    I2C_Master_Write((MPL3115A2_ADDRESS << 1) | 1); // Read mode

    for (int i = 0; i < 5; i++) {
        buffer[i] = I2C_Read_Byte();
        if (i < 4) {
            I2C_ACK();
        } else {
            I2C_NACK();
        }
    }
    I2C_Master_Stop();
    switch (value) {
    case MPL3115A2_PRESSURE:
        pressure = ((uint32_t)buffer[0] << 16) | ((uint32_t)buffer[1] << 8) | (uint32_t)(buffer[2]);
        return (float)pressure / 6400.0; // Conversion to hPa
    case MPL3115A2_ALTITUDE:
        altitude = ((uint32_t)buffer[0] << 24) | ((uint32_t)buffer[1] << 16) | ((uint32_t)buffer[2] << 8);
        return (float)altitude / 65536.0;
    case MPL3115A2_TEMPERATURE:
    default:
        temperature = ((uint16_t)buffer[3] << 8) | buffer[4];
        return (float)temperature / 256.0;
    }
}

float MPL3115A2_GetPressure(void) {
    if (currentMode != MPL3115A2_BAROMETER) {
        MPL3115A2_SetMode(MPL3115A2_BAROMETER);
    }
    MPL3115A2_StartOneShot();
    while (!MPL3115A2_ConversionComplete()) {
        __delay_ms(10);
    }
    return MPL3115A2_GetLastConversionResults(MPL3115A2_PRESSURE);
}

float MPL3115A2_GetAltitude(void) {
    if (currentMode != MPL3115A2_ALTIMETER) {
        MPL3115A2_SetMode(MPL3115A2_ALTIMETER);
    }
    MPL3115A2_StartOneShot();
    while (!MPL3115A2_ConversionComplete()) {
        __delay_ms(10);
    }
    return MPL3115A2_GetLastConversionResults(MPL3115A2_ALTITUDE);
}

float MPL3115A2_GetTemperature(void) {
    MPL3115A2_StartOneShot();
    while (!MPL3115A2_ConversionComplete()) {
        __delay_ms(10);
    }
    return MPL3115A2_GetLastConversionResults(MPL3115A2_TEMPERATURE);
}

void MPL3115A2_SetAltitudeOffset(int8_t offset) {
    // Write the altitude offset to the MPL3115A2_OFF_H register
    MPL3115A2_WriteRegister(MPL3115A2_OFF_H, (uint8_t)offset);
}

void MPL3115A2_SetSeaPressure(float SLP) {
    // Convert sea level pressure (hPa) to the required format
    // MPL3115A2 expects values in 2 Pa increments, which is SLP * 50
    uint16_t bar_in = (uint16_t)(SLP * 50); 
    // Write the converted value to the BAR_IN_MSB and BAR_IN_LSB registers
    MPL3115A2_WriteRegister(MPL3115A2_BAR_IN_MSB, (uint8_t)(bar_in >> 8)); // Most significant byte
    MPL3115A2_WriteRegister(MPL3115A2_BAR_IN_LSB, (uint8_t)(bar_in & 0xFF)); // Least significant byte
}

uint8_t MPL3115A2_ReadRegister(uint8_t reg) {
    uint8_t data;
    I2C_Master_Start();
    I2C_Master_Write((MPL3115A2_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(reg);
    I2C_Master_RepeatedStart();
    I2C_Master_Write((MPL3115A2_ADDRESS << 1) | 1); // Read mode
    data = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();

    return data;
}

void MPL3115A2_WriteRegister(uint8_t reg, uint8_t value) {
    I2C_Master_Start();
    I2C_Master_Write((MPL3115A2_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(reg);
    I2C_Master_Write(value);
    I2C_Master_Stop();
}

void MPL3115A2_SetMode(mpl3115a2_mode_t mode) {
    _ctrl_reg1.reg = MPL3115A2_ReadRegister(MPL3115A2_CTRL_REG1);
    _ctrl_reg1.bit.ALT = (mode == MPL3115A2_ALTIMETER) ? 1 : 0;
    MPL3115A2_WriteRegister(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);
    currentMode = mode;
}

void MPL3115A2_StartOneShot(void) {
    do {
        _ctrl_reg1.reg = MPL3115A2_ReadRegister(MPL3115A2_CTRL_REG1);
        __delay_ms(10);
    } while (_ctrl_reg1.bit.OST);

    _ctrl_reg1.bit.OST = 1;
    MPL3115A2_WriteRegister(MPL3115A2_CTRL_REG1, _ctrl_reg1.reg);
}

bool MPL3115A2_ConversionComplete(void) {
    // Check the PTDR bit in the STATUS register
    uint8_t status = MPL3115A2_ReadRegister(MPL3115A2_REGISTER_STATUS);
    return (status & MPL3115A2_REGISTER_STATUS_PTDR) != 0;
}
