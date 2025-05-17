
#ifndef __MPL3115A2__
#define __MPL3115A2__

#include "main.h"
#include "i2c.h"

#define MPL3115A2_ADDRESS 0x60 ///< default I2C address 1100000

/** MPL3115A2 registers **/
enum {
    MPL3115A2_REGISTER_STATUS = 0x00,
    
    MPL3115A2_REGISTER_PRESSURE_MSB = 0x01,
    MPL3115A2__EGISTER_PRESSURE_CSB = 0x02,
    MPL3115A2_REGISTER_PRESSURE_LSB = 0x03,
    
    MPL3115A2_REGISTER_TEMP_MSB = 0x04,
    MPL3115A2_REGISTER_TEMP_LSB = 0x05,
    
    MPL3115A2_REGISTER_DR_STATUS = 0x06,
    
    MPL3115A2_OUT_P_DELTA_MSB = 0x07,
    MPL3115A2_OUT_P_DELTA_CSB = 0x08,
    MPL3115A2_OUT_P_DELTA_LSB = 0x09,
    
    MPL3115A2_OUT_T_DELTA_MSB = 0x0A,
    MPL3115A2_OUT_T_DELTA_LSB = 0x0B,
    
    MPL3115A2_WHOAMI = 0x0C,
    
    MPL3115A2_BAR_IN_MSB = 0x14,
    MPL3115A2_BAR_IN_LSB = 0x15,
    
    MPL3115A2_OFF_H = 0x2D,
    };
    
/** MPL3115A2 status register bits **/
enum {
    MPL3115A2_REGISTER_STATUS_TDR = 0x02,
    MPL3115A2_REGISTER_STATUS_PDR = 0x04,
    MPL3115A2_REGISTER_STATUS_PTDR = 0x08,
    };

/** MPL3115A2 PT DATA register bits **/
enum {
    MPL3115A2_PT_DATA_CFG = 0x13,
    MPL3115A2_PT_DATA_CFG_TDEFE = 0x01,
    MPL3115A2_PT_DATA_CFG_PDEFE = 0x02,
    MPL3115A2_PT_DATA_CFG_DREM = 0x04,
    };

/** MPL3115A2 control registers **/
enum {
    MPL3115A2_CTRL_REG1 = 0x26,
    MPL3115A2_CTRL_REG2 = 0x27,
    MPL3115A2_CTRL_REG3 = 0x28,
    MPL3115A2_CTRL_REG4 = 0x29,
    MPL3115A2_CTRL_REG5 = 0x2A,
    };

/** MPL3115A2 control register bits **/
enum {
    MPL3115A2_CTRL_REG1_SBYB = 0x01,
    MPL3115A2_CTRL_REG1_OST = 0x02,
    MPL3115A2_CTRL_REG1_RST = 0x04,
    MPL3115A2_CTRL_REG1_RAW = 0x40,
    MPL3115A2_CTRL_REG1_ALT = 0x80,
    MPL3115A2_CTRL_REG1_BAR = 0x00,
    };
                    
/** MPL3115A2 oversample values **/
enum {
    MPL3115A2_CTRL_REG1_OS1 = 0x00,
    MPL3115A2_CTRL_REG1_OS2 = 0x08,
    MPL3115A2_CTRL_REG1_OS4 = 0x10,
    MPL3115A2_CTRL_REG1_OS8 = 0x18,
    MPL3115A2_CTRL_REG1_OS16 = 0x20,
    MPL3115A2_CTRL_REG1_OS32 = 0x28,
    MPL3115A2_CTRL_REG1_OS64 = 0x30,
    MPL3115A2_CTRL_REG1_OS128 = 0x38,
};
                        
/** MPL3115A2 measurement modes **/
typedef enum {
    MPL3115A2_BAROMETER = 0,
    MPL3115A2_ALTIMETER,
} mpl3115a2_mode_t;

/** MPL3115A2 measurement types **/
typedef enum {
    MPL3115A2_PRESSURE,
    MPL3115A2_ALTITUDE,
    MPL3115A2_TEMPERATURE,
} mpl3115a2_meas_t;

#define MPL3115A2_REGISTER_STARTCONVERSION 0x12 ///< start conversion

// Function prototypes
uint8_t MPL3115A2_ReadRegister(uint8_t reg);
void MPL3115A2_WriteRegister(uint8_t reg, uint8_t value);
bool MPL3115A2_Init(void);
void MPL3115A2_SetMode(mpl3115a2_mode_t mode);
void MPL3115A2_StartOneShot(void);
bool MPL3115A2_ConversionComplete(void) ;
float MPL3115A2_GetLastConversionResults(mpl3115a2_meas_t value);

float MPL3115A2_GetPressure(void);
float MPL3115A2_GetAltitude(void);
float MPL3115A2_GetTemperature(void);

int8_t MPL3115A2_GetAltitudeOffset(void);
void MPL3115A2_SetSeaPressure(float SLP);
void MPL3115A2_SetAltitudeOffset(int8_t offset);

// Data structure and variables
typedef union {
    struct {
        uint8_t SBYB : 1;
        uint8_t OST : 1;
        uint8_t RST : 1;
        uint8_t OS : 3;
        uint8_t RAW : 1;
        uint8_t ALT : 1;
        } bit;
    uint8_t reg;
} ctrl_reg1;

ctrl_reg1 _ctrl_reg1;
mpl3115a2_mode_t currentMode;
                                                        
#endif
                                                        
                                                        
