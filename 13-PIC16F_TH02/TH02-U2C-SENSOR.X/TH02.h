
#ifndef TH02_H
#define TH02_H

#include "i2c.h"

// TH02 I2C Device address
#define TH02_I2C_ADDR 0x40

// TH02 Registers addresses
#define TH02_STATUS 0
#define TH02_DATAh  1
#define TH02_DATAl  2
#define TH02_CONFIG 3
#define TH02_ID     17

// Bit definitions for TH02 registers
#define TH02_STATUS_RDY    0x01
#define TH02_CONFIG_START  0x01
#define TH02_CONFIG_HEAT   0x02
#define TH02_CONFIG_TEMP   0x10
#define TH02_CONFIG_HUMI   0x00
#define TH02_CONFIG_FAST   0x20

// Linearization coefficients
#define TH02_A0   -4.7844f
#define TH02_A1    0.4008f
#define TH02_A2   -0.00393f

// Temperature compensation coefficients
#define TH02_Q0   0.1973f
#define TH02_Q1   0.00237f

// Error and special values
#define TH02_UNDEFINED_VALUE    12345
#define TH02_UNINITIALIZED_TEMP 55555
#define TH02_UNINITIALIZED_RH   1111

#define TH02_CONVERSION_TIME_OUT  100

// Class structure for TH02 operations
typedef struct {
    uint8_t address;  // I2C address
    int32_t last_temp; // Last measured temperature (for linearization)
    int32_t last_rh;   // Last measured RH
    } TH02_t;
    
    // Function prototypes
    uint8_t TH02_WriteRegister(uint8_t reg, uint8_t value);
    uint8_t TH02_ReadRegister(uint8_t reg, uint8_t *value);
    uint8_t TH02_StartTempConversion(uint8_t fastmode, uint8_t heater);
    uint8_t TH02_StartRHConversion(uint8_t fastmode, uint8_t heater);
    uint8_t TH02_WaitForReady(void);
    int16_t roundInt(float value);
    int16_t TH02_GetConversionValue(void);
    int16_t TH02_GetCompensatedRH(uint8_t round_flag);
#endif // TH02_H
    
    
