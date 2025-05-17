
#include "INA219.h"

static uint16_t ina219_calValue;
static uint32_t ina219_currentDivider_mA;
static float ina219_powerMultiplier_mW;

bool INA219_Init(void) {
    I2C_Master_Init(); // Initialize I2C
    INA219_SetCalibration_32V_2A();
    return true;
    }

void INA219_SetCalibration_32V_2A(void) {
    ina219_calValue = 4096;
    ina219_currentDivider_mA = 10;
    ina219_powerMultiplier_mW = 2.0f;
    
    // Write calibration register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0);
    I2C_Master_Write(INA219_REG_CALIBRATION);
    I2C_Master_Write((ina219_calValue >> 8) & 0xFF);
    I2C_Master_Write(ina219_calValue & 0xFF);
    I2C_Master_Stop();
    
    // Write config register
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
            INA219_CONFIG_GAIN_8_320MV |
            INA219_CONFIG_BADCRES_12BIT |
            INA219_CONFIG_SADCRES_12BIT_1S_532US |
            INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0);
    I2C_Master_Write(INA219_REG_CONFIG);
    I2C_Master_Write((config >> 8) & 0xFF);
    I2C_Master_Write(config & 0xFF);
    I2C_Master_Stop();
    }

void INA219_SetCalibration_32V_1A(void) {
    ina219_calValue = 10240;
    ina219_currentDivider_mA = 25;
    ina219_powerMultiplier_mW = 0.8f;
    
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0);
    I2C_Master_Write(INA219_REG_CALIBRATION);
    I2C_Master_Write((ina219_calValue >> 8) & 0xFF);
    I2C_Master_Write(ina219_calValue & 0xFF);
    I2C_Master_Stop();
    
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_32V |
            INA219_CONFIG_GAIN_8_320MV |
            INA219_CONFIG_BADCRES_12BIT |
            INA219_CONFIG_SADCRES_12BIT_1S_532US |
            INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0);
    I2C_Master_Write(INA219_REG_CONFIG);
    I2C_Master_Write((config >> 8) & 0xFF);
    I2C_Master_Write(config & 0xFF);
    I2C_Master_Stop();
    }

void INA219_SetCalibration_16V_400mA(void) {
    ina219_calValue = 8192;
    ina219_currentDivider_mA = 20;
    ina219_powerMultiplier_mW = 1.0f;
    
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0);
    I2C_Master_Write(INA219_REG_CALIBRATION);
    I2C_Master_Write((ina219_calValue >> 8) & 0xFF);
    I2C_Master_Write(ina219_calValue & 0xFF);
    I2C_Master_Stop();
    
    uint16_t config = INA219_CONFIG_BVOLTAGERANGE_16V |
            INA219_CONFIG_GAIN_1_40MV |
            INA219_CONFIG_BADCRES_12BIT |
            INA219_CONFIG_SADCRES_12BIT_1S_532US |
            INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
    
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0);
    I2C_Master_Write(INA219_REG_CONFIG);
    I2C_Master_Write((config >> 8) & 0xFF);
    I2C_Master_Write(config & 0xFF);
    I2C_Master_Stop();
    }

float INA219_GetBusVoltage_V(void) {
    uint16_t rawBusVoltage;
    
    // Step 1: Set the register pointer to the bus voltage register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(INA219_REG_BUSVOLTAGE);    // Pointer to Bus Voltage Register
    I2C_Master_Stop();                          // Ensure the pointer is set
    
    // Step 2: Read 2 bytes from the bus voltage register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 1); // Read mode
    rawBusVoltage = ((uint16_t)I2C_Read_Byte() << 8); // Read MSByte and shift
    I2C_ACK();                                 // Send ACK
    rawBusVoltage |= I2C_Read_Byte();                // Read LSByte
    I2C_NACK();                                      // Send NACK to end transmission
    I2C_Master_Stop();
    
    // Step 3: Convert the raw value to voltage
    // The datasheet specifies:
    //   - Shift the raw value right by 3 bits to remove flags
    //   - Multiply by 4 mV (0.004 V) per LSB
    return (rawBusVoltage >> 3) * 0.004f;
    }


float INA219_GetShuntVoltage_mV(void) {
    uint16_t rawShuntVoltage;
    
    // Step 1: Set the register pointer to the shunt voltage register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(INA219_REG_SHUNTVOLTAGE);  // Pointer to Shunt Voltage Register
    I2C_Master_Stop();                          // Ensure the pointer is set
    
    // Step 2: Read 2 bytes from the shunt voltage register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 1); // Read mode
    rawShuntVoltage = ((uint16_t)I2C_Read_Byte() << 8); // Read MSByte and shift
    I2C_ACK();                                 // Send ACK
    rawShuntVoltage |= I2C_Read_Byte();              // Read LSByte
    I2C_NACK();                                      // Send NACK to end transmission
    I2C_Master_Stop();
    
    // Step 3: Convert the raw value to millivolts (LSB = 10uV)
    return rawShuntVoltage * 0.01f;
    }

float INA219_GetCurrent_mA(void) {
    uint16_t rawCurrent;
    
    // Step 1: Write calibration value to the calibration register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(INA219_REG_CALIBRATION);   // Pointer to Calibration Register
    I2C_Master_Write((ina219_calValue >> 8) & 0xFF); // MSB of calibration value
    I2C_Master_Write(ina219_calValue & 0xFF);        // LSB of calibration value
    I2C_Master_Stop();
    
    // Step 2: Set the register pointer to the current register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(INA219_REG_CURRENT);       // Pointer to Current Register
    I2C_Master_Stop();
    
    // Step 3: Read 2 bytes from the current register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 1); // Read mode
    rawCurrent = ((uint16_t)I2C_Read_Byte() << 8); // Read MSByte and shift
    I2C_ACK();                                 // Send ACK
    rawCurrent |= I2C_Read_Byte();                   // Read LSByte
    I2C_NACK();                                      // Send NACK to end transmission
    I2C_Master_Stop();
    
    // Step 4: Convert raw current value to milliamps
    // Divide by the current divider (set during calibration)
    return (float)rawCurrent / ina219_currentDivider_mA;
    }


float INA219_GetPower_mW(void) {
    uint16_t rawPower;
    
    // Step 1: Write calibration value to the calibration register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(INA219_REG_CALIBRATION);   // Pointer to Calibration Register
    I2C_Master_Write((ina219_calValue >> 8) & 0xFF); // MSB of calibration value
    I2C_Master_Write(ina219_calValue & 0xFF);        // LSB of calibration value
    I2C_Master_Stop();
    
    // Step 2: Set the register pointer to the power register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(INA219_REG_POWER);         // Pointer to Power Register
    I2C_Master_Stop();
    
    // Step 3: Read 2 bytes from the power register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 1); // Read mode
    rawPower = ((uint16_t)I2C_Read_Byte() << 8); // Read MSByte and shift
    I2C_ACK();                                // Send ACK
    rawPower |= I2C_Read_Byte();                    // Read LSByte
    I2C_NACK();                                     // Send NACK to end transmission
    I2C_Master_Stop();
    
    // Step 4: Convert raw power value to milliwatts
    // Multiply by the power multiplier (set during calibration)
    return rawPower * ina219_powerMultiplier_mW;
    }


void INA219_PowerSave(bool on) {
    uint16_t config;
    
    // Step 1: Read the current configuration register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(INA219_REG_CONFIG);        // Pointer to Configuration Register
    I2C_Master_Stop();                          // Ensure the pointer is set
    
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 1); // Read mode
    config = ((uint16_t)I2C_Read_Byte() << 8);   // Read MSByte and shift
    I2C_ACK();                                 // Send ACK
    config |= I2C_Read_Byte();                   // Read LSByte
    I2C_NACK();                                  // Send NACK to end transmission
    I2C_Master_Stop();
    
    // Step 2: Modify the operating mode for power saving
    if (on) {
        // Disable continuous mode (set to power-down mode)
        config &= ~INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
        config |= INA219_CONFIG_MODE_POWERDOWN;
        } else {
        // Enable continuous shunt and bus voltage measurement
        config &= ~INA219_CONFIG_MODE_MASK; // Clear mode bits
        config |= INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS;
        }
    
    // Step 3: Write the updated configuration back to the register
    I2C_Master_Start();
    I2C_Master_Write((INA219_ADDRESS << 1) | 0); // Write mode
    I2C_Master_Write(INA219_REG_CONFIG);        // Pointer to Configuration Register
    I2C_Master_Write((config >> 8) & 0xFF);     // MSB of updated configuration
    I2C_Master_Write(config & 0xFF);            // LSB of updated configuration
    I2C_Master_Stop();
    }


