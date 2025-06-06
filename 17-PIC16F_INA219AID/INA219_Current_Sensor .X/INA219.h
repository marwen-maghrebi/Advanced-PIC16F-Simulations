
// INA219.h
#ifndef INA219_H
#define INA219_H

#include <stdint.h>
#include "i2c.h"

// INA219 I2C Address
#define INA219_ADDRESS 0x40

// Register Addresses
#define INA219_REG_CONFIG 0x00
#define INA219_REG_SHUNTVOLTAGE 0x01
#define INA219_REG_BUSVOLTAGE 0x02
#define INA219_REG_POWER 0x03
#define INA219_REG_CURRENT 0x04
#define INA219_REG_CALIBRATION 0x05

// Configuration Register Masks
#define INA219_CONFIG_RESET 0x8000
#define INA219_CONFIG_BVOLTAGERANGE_MASK 0x2000
#define INA219_CONFIG_GAIN_MASK 0x1800
#define INA219_CONFIG_BADCRES_MASK 0x0780
#define INA219_CONFIG_SADCRES_MASK 0x0078
#define INA219_CONFIG_MODE_MASK 0x0007

// Bus Voltage Range Values
#define INA219_CONFIG_BVOLTAGERANGE_16V 0x0000
#define INA219_CONFIG_BVOLTAGERANGE_32V 0x2000

// Gain Values
#define INA219_CONFIG_GAIN_1_40MV 0x0000
#define INA219_CONFIG_GAIN_2_80MV 0x0800
#define INA219_CONFIG_GAIN_4_160MV 0x1000
#define INA219_CONFIG_GAIN_8_320MV 0x1800

// Bus ADC Resolution Values
#define INA219_CONFIG_BADCRES_9BIT 0x0000
#define INA219_CONFIG_BADCRES_10BIT 0x0080
#define INA219_CONFIG_BADCRES_11BIT 0x0100
#define INA219_CONFIG_BADCRES_12BIT 0x0180
#define INA219_CONFIG_BADCRES_12BIT_2S_1060US 0x0480
#define INA219_CONFIG_BADCRES_12BIT_4S_2130US 0x0500
#define INA219_CONFIG_BADCRES_12BIT_8S_4260US 0x0580
#define INA219_CONFIG_BADCRES_12BIT_16S_8510US 0x0600
#define INA219_CONFIG_BADCRES_12BIT_32S_17MS 0x0680
#define INA219_CONFIG_BADCRES_12BIT_64S_34MS 0x0700
#define INA219_CONFIG_BADCRES_12BIT_128S_69MS 0x0780

// Shunt ADC Resolution Values
#define INA219_CONFIG_SADCRES_9BIT_1S_84US 0x0000
#define INA219_CONFIG_SADCRES_10BIT_1S_148US 0x0008
#define INA219_CONFIG_SADCRES_11BIT_1S_276US 0x0010
#define INA219_CONFIG_SADCRES_12BIT_1S_532US 0x0018
#define INA219_CONFIG_SADCRES_12BIT_2S_1060US 0x0048
#define INA219_CONFIG_SADCRES_12BIT_4S_2130US 0x0050
#define INA219_CONFIG_SADCRES_12BIT_8S_4260US 0x0058
#define INA219_CONFIG_SADCRES_12BIT_16S_8510US 0x0060
#define INA219_CONFIG_SADCRES_12BIT_32S_17MS 0x0068
#define INA219_CONFIG_SADCRES_12BIT_64S_34MS 0x0070
#define INA219_CONFIG_SADCRES_12BIT_128S_69MS 0x0078

// Operating Mode Values
#define INA219_CONFIG_MODE_POWERDOWN 0x00
#define INA219_CONFIG_MODE_SVOLT_TRIGGERED 0x01
#define INA219_CONFIG_MODE_BVOLT_TRIGGERED 0x02
#define INA219_CONFIG_MODE_SANDBVOLT_TRIGGERED 0x03
#define INA219_CONFIG_MODE_ADCOFF 0x04
#define INA219_CONFIG_MODE_SVOLT_CONTINUOUS 0x05
#define INA219_CONFIG_MODE_BVOLT_CONTINUOUS 0x06
#define INA219_CONFIG_MODE_SANDBVOLT_CONTINUOUS 0x07

// Function Prototypes
bool INA219_Init(void);
void INA219_SetCalibration_32V_2A(void);
void INA219_SetCalibration_32V_1A(void);
void INA219_SetCalibration_16V_400mA(void);
float INA219_GetBusVoltage_V(void);
float INA219_GetShuntVoltage_mV(void);
float INA219_GetCurrent_mA(void);
float INA219_GetPower_mW(void);
void INA219_PowerSave(bool on);

#endif // INA219_H

