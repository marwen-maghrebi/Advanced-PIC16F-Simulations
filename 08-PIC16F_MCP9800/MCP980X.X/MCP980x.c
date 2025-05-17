#include "MCP980x.h"

void MCP9800_Init(void) {
    I2C_Master_Init();
    MCP9800_WriteConfig(0x00);
    }

void MCP9800_WriteConfig(uint8_t value) {
    I2C_Master_Start();
    I2C_Master_Write(MCP9800_ADDRESS << 1);
    I2C_Master_Write(CONFIG_REG);
    I2C_Master_Write(value);
    I2C_Master_Stop();
    }

uint8_t MCP9800_ReadConfig(void) {
    uint8_t config_value;
    
    I2C_Master_Start();
    I2C_Master_Write(MCP9800_ADDRESS << 1);
    I2C_Master_Write(CONFIG_REG);
    I2C_Master_RepeatedStart();
    I2C_Master_Write((MCP9800_ADDRESS << 1) | 0x01);
    config_value = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    
    return config_value;
    }

uint8_t MCP9800_GetConfigOneShot(void) {
    uint8_t config = MCP9800_ReadConfig();
    return (config >> CONFIG_ONE_SHOT) & 0x01;
    }

uint8_t MCP9800_GetConfigResolution(void) {
    uint8_t config = MCP9800_ReadConfig();
    return (config >> CONFIG_ADC_RES) & 0x03;
    }

uint8_t MCP9800_GetConfigFaultQueue(void) {
    uint8_t config = MCP9800_ReadConfig();
    return (config >> CONFIG_FAULT_QUEUE) & 0x03;
    }

uint8_t MCP9800_GetConfigAlertPolarity(void) {
    uint8_t config = MCP9800_ReadConfig();
    return (config >> CONFIG_ALERT_POL) & 0x01;
    }

uint8_t MCP9800_GetConfigCompIntMode(void) {
    uint8_t config = MCP9800_ReadConfig();
    return (config >> CONFIG_COMP_INT) & 0x01;
    }

uint8_t MCP9800_GetConfigShutdown(void) {
    uint8_t config = MCP9800_ReadConfig();
    return (config >> CONFIG_SHUTDOWN) & 0x01;
    }

void MCP9800_SetConfigOneShot(uint8_t enable) {
    uint8_t config = MCP9800_ReadConfig();
    if (enable) {
        config |= (1 << CONFIG_ONE_SHOT);
        } else {
        config &= ~(1 << CONFIG_ONE_SHOT);
        }
    MCP9800_WriteConfig(config);
    }

void MCP9800_SetConfigResolution(uint8_t resolution) {
    uint8_t config = MCP9800_ReadConfig();
    config &= ~(0x03 << CONFIG_ADC_RES);
    config |= (resolution & 0x03) << CONFIG_ADC_RES;
    MCP9800_WriteConfig(config);
    }

void MCP9800_SetConfigFaultQueue(uint8_t fault_queue) {
    uint8_t config = MCP9800_ReadConfig();
    config &= ~(0x03 << CONFIG_FAULT_QUEUE);
    config |= (fault_queue & 0x03) << CONFIG_FAULT_QUEUE;
    MCP9800_WriteConfig(config);
    }

void MCP9800_SetConfigAlertPolarity(uint8_t polarity) {
    uint8_t config = MCP9800_ReadConfig();
    if (polarity) {
        config |= (1 << CONFIG_ALERT_POL);
        } else {
        config &= ~(1 << CONFIG_ALERT_POL);
        }
    MCP9800_WriteConfig(config);
    }

void MCP9800_SetConfigCompIntMode(uint8_t mode) {
    uint8_t config = MCP9800_ReadConfig();
    if (mode) {
        config |= (1 << CONFIG_COMP_INT);
        } else {
        config &= ~(1 << CONFIG_COMP_INT);
        }
    MCP9800_WriteConfig(config);
    }

void MCP9800_SetConfigShutdown(uint8_t enable) {
    uint8_t config = MCP9800_ReadConfig();
    if (enable) {
        config |= (1 << CONFIG_SHUTDOWN);
        } else {
        config &= ~(1 << CONFIG_SHUTDOWN);
        }
    MCP9800_WriteConfig(config);
    }

void MCP9800_SetLIMIT_Temp(int16_t temp_value) {
    uint8_t msb, lsb;
    int16_t temp_raw_value = temp_value * 2;
    msb = (uint8_t)((temp_raw_value >> 1) & 0xFF);
    lsb = (uint8_t)((temp_raw_value & 0x01) << 7);
    
    I2C_Master_Start();
    I2C_Master_Write(MCP9800_ADDRESS << 1);
    I2C_Master_Write(REG_LIMIT);
    I2C_Master_Write(msb);
    I2C_Master_Write(lsb);
    I2C_Master_Stop();
    }

int16_t MCP9800_GetLIMIT_Temp(void) {
    uint8_t temp_msb, temp_lsb;
    int16_t temperature, temp_raw_value;
    
    I2C_Master_Start();
    I2C_Master_Write(MCP9800_ADDRESS << 1);
    I2C_Master_Write(REG_LIMIT);
    I2C_Master_RepeatedStart();
    
    I2C_Master_Write((MCP9800_ADDRESS << 1) | 0x01);
    temp_msb = I2C_Read_Byte();
    I2C_ACK();
    temp_lsb = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    temp_raw_value = ((int16_t)temp_msb << 1) | ((int16_t)(temp_lsb >> 7) & 0x01);
    temperature = temp_raw_value * 0.5;
    return (temperature);
    }

void MCP9800_SetHYST_Temp(int16_t temp_value) {
    uint8_t msb, lsb;
    int16_t temp_raw_value = temp_value * 2;
    msb = (uint8_t)((temp_raw_value >> 1) & 0xFF);
    lsb = (uint8_t)((temp_raw_value & 0x01) << 7);
    
    I2C_Master_Start();
    I2C_Master_Write(MCP9800_ADDRESS << 1);
    I2C_Master_Write(REG_HYSTERESIS);
    I2C_Master_Write(msb);
    I2C_Master_Write(lsb);
    I2C_Master_Stop();
    }

int16_t MCP9800_GetHYST_Temp(void) {
    uint8_t temp_msb, temp_lsb;
    int16_t temperature, temp_raw_value;
    
    I2C_Master_Start();
    I2C_Master_Write(MCP9800_ADDRESS << 1);
    I2C_Master_Write(REG_HYSTERESIS);
    I2C_Master_RepeatedStart();
    
    I2C_Master_Write((MCP9800_ADDRESS << 1) | 0x01);
    temp_msb = I2C_Read_Byte();
    I2C_ACK();
    temp_lsb = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    temp_raw_value = ((int16_t)temp_msb << 1) | ((int16_t)(temp_lsb >> 7) & 0x01);
    temperature = temp_raw_value * 0.5;
    return (temperature);
    }

int16_t MCP9800_ReadTemperature(void) {
    uint8_t temp_msb, temp_lsb;
    int16_t temperature;
    
    I2C_Master_Start();
    I2C_Master_Write(MCP9800_ADDRESS << 1);
    I2C_Master_Write(TEMP_REGISTER);
    I2C_Master_RepeatedStart();
    I2C_Master_Write((MCP9800_ADDRESS << 1) | 0x01);
    temp_msb = I2C_Read_Byte();
    I2C_ACK();
    temp_lsb = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    temperature = (((temp_msb << 8) | temp_lsb) >> 4);
    return (temperature * 0.0625);
    }

