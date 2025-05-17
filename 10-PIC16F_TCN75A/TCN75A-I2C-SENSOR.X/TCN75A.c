
#include "TCN75A.h"

static uint8_t TCN75A_Address;

// Function declarations for TCN75A_SetTemp and TCN75A_GetTemp
void TCN75A_Init(uint8_t adr) {
    TCN75A_Address = adr;
    I2C_Master_Init();
    
    }

float TCN75A_ReadTemperature() {
    uint8_t tempHigh, tempLow;
    int16_t tempRaw;  // We still use a signed 16-bit variable to handle negative values
    float temp;
    
    // Start communication with TCN75A
    I2C_Master_Start();
    I2C_Master_Write((unsigned char)(TCN75A_Address << 1)); // Write mode
    I2C_Master_Write(TCN75A_TEMP_REG); // Temperature register
    I2C_Master_Stop();
    
    // Start read communication to get the temperature data
    I2C_Master_Start();
    I2C_Master_Write((unsigned char)((TCN75A_Address << 1) | 1)); // Read mode
    
    // Read the high and low bytes
    tempHigh = I2C_Read_Byte();
    I2C_ACK();
    tempLow = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    
    // Combine high and low bytes into a 12-bit value
    tempRaw = (int16_t)((tempHigh << 4) | (tempLow >> 4)); // Shift high byte by 4 and combine with upper nibble of low byte
    
    // Handle negative temperature (two's complement check)
    if (tempRaw & 0x0800) {  // Check if the 12th bit (MSB) is set
        tempRaw |= 0xF000;  // Sign extend the 12-bit value to 16 bits
        }
    
    // Convert the raw temperature value to Celsius (0.0625C per bit)
    temp = tempRaw * 0.0625f;
    
    return temp;
    }

void TCN75A_SetHystTemp(float val) {
    TCN75A_SetTemp(TCN75A_THYST_REG, val);
    }

void TCN75A_SetLimitTemp(float val) {
    TCN75A_SetTemp(TCN75A_TLIMIT_REG, val);
    }

void TCN75A_SetRangeTemp(float val_down, float val_up) {
    TCN75A_SetHystTemp(val_down);
    TCN75A_SetLimitTemp(val_up);
    }

float TCN75A_GetLimitTemp() {
    return TCN75A_GetTemp(TCN75A_TLIMIT_REG);
    }

float TCN75A_GetHystTemp() {
    return TCN75A_GetTemp(TCN75A_THYST_REG);
    }

void TCN75A_SetTemp(uint8_t reg, float value) {
    // Ensure value is within valid temperature range
    if(value < -40.0f) value = -40.0f;
    if(value > 125.0f) value = 125.0f;
    
    uint8_t tempHigh = (uint8_t)value;
    uint8_t tempLow = (uint8_t)((value - (float)tempHigh) >= 0.5f ? 0x80 : 0x00);
    
    I2C_Master_Start();
    I2C_Master_Write((unsigned char)(TCN75A_Address << 1));
    I2C_Master_Write(reg);
    I2C_Master_Write(tempHigh);
    I2C_Master_Write(tempLow);
    I2C_Master_Stop();
    }

float TCN75A_GetTemp(uint8_t reg) {
    int8_t tempHigh;
    uint8_t tempLow;
    float temp;
    
    I2C_Master_Start();
    I2C_Master_Write((unsigned char)(TCN75A_Address << 1));
    I2C_Master_Write(reg);
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write((unsigned char)((TCN75A_Address << 1) | 1));
    tempHigh = (int8_t)I2C_Read_Byte(); // Cast to int8_t to ensure signedness
    I2C_ACK();
    tempLow = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    
    temp = (float)tempHigh + ((tempLow >> 7) ? 0.5f : 0.0f); // Add 'f' for float precision
    return temp;
    }

uint8_t TCN75A_ReadConfig() {
    uint8_t config;
    
    I2C_Master_Start();
    I2C_Master_Write((unsigned char)(TCN75A_Address << 1)); // Write mode
    I2C_Master_Write(TCN75A_CONFIG_REG);
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write((unsigned char)((TCN75A_Address << 1) | 1)); // Read mode
    config = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    
    return config;
    }

void TCN75A_WriteConfig(uint8_t config) {
    I2C_Master_Start();
    I2C_Master_Write((unsigned char)(TCN75A_Address << 1));
    I2C_Master_Write(TCN75A_CONFIG_REG);
    I2C_Master_Write(config);
    I2C_Master_Stop();
    }

void TCN75A_SetOneShot(uint8_t sw) {
    uint8_t config = TCN75A_ReadConfig();
    if (sw) config |= (1 << 7); else config &= ~(1 << 7);
    TCN75A_WriteConfig(config);
    }

void TCN75A_SetResolution(uint8_t val) {
    uint8_t config = TCN75A_ReadConfig();
    val = val & 0x03;
    config = (config & 0x9F) | (uint8_t)(val << 5); // Add explicit cast to uint8_t
    TCN75A_WriteConfig(config);
    }

void TCN75A_SetFaultQueue(uint8_t val) {
    uint8_t config = TCN75A_ReadConfig();
    val = val & 0x03;
    config = (config & 0xE7) | (uint8_t)(val << 3); // Add explicit cast to uint8_t
    TCN75A_WriteConfig(config);
    }

void TCN75A_SetAlertPolarity(uint8_t sw) {
    uint8_t config = TCN75A_ReadConfig();
    if (sw) config |= (1 << 2); else config &= ~(1 << 2);
    TCN75A_WriteConfig(config);
    }

void TCN75A_SetAlertMode(uint8_t sw) {
    uint8_t config = TCN75A_ReadConfig();
    if (sw) config |= (1 << 1); else config &= ~(1 << 1);
    TCN75A_WriteConfig(config);
    }

void TCN75A_SetShutdown(uint8_t sw) {
    uint8_t config = TCN75A_ReadConfig();
    if (sw) config |= (1 << 0); else config &= ~(1 << 0);
    TCN75A_WriteConfig(config);
    }

int8_t TCN75A_CheckConfig(uint8_t op) {
    if(op > 6) return -1;
    uint8_t config = TCN75A_ReadConfig();
    return (int8_t)((config >> op) & 0x01); // Cast to int8_t to match return type
    }

// Define TCN75A_CheckAlert function
uint8_t TCN75A_CheckAlert(void) {
    uint8_t config;
    config = TCN75A_ReadConfig();
    return (config & 0x01); // Return 1 if alert is active, 0 if no alert
    }

