
#ifndef BMP085_H
#define BMP085_H

#include "i2c.h"
// Declare global/static variables
static int16_t ac1, ac2, ac3, b1, b2, mb, mc, md;
static uint16_t ac4, ac5, ac6;
static uint8_t oversampling = 3;


// I2C address for BMP085
#define BMP085_I2CADDR 0x77

// Calibration register addresses
#define BMP085_CAL_AC1 0xAA
#define BMP085_CAL_AC2 0xAC
#define BMP085_CAL_AC3 0xAE
#define BMP085_CAL_AC4 0xB0
#define BMP085_CAL_AC5 0xB2
#define BMP085_CAL_AC6 0xB4
#define BMP085_CAL_B1  0xB6
#define BMP085_CAL_B2  0xB8
#define BMP085_CAL_MB  0xBA
#define BMP085_CAL_MC  0xBC
#define BMP085_CAL_MD  0xBE

// Control register and commands
#define BMP085_CONTROL         0xF4
#define BMP085_TEMPDATA        0xF6
#define BMP085_PRESSUREDATA    0xF6
#define BMP085_READTEMPCMD     0x2E
#define BMP085_READPRESSURECMD 0x34
#define chip_id                0xD0 // use BMP085_Read8 function result should be 0x55
// Function prototypes
void     BMP085_Write8(uint8_t addr, uint8_t data);
uint8_t  BMP085_Read8(uint8_t addr);
uint16_t BMP085_Read16(uint8_t addr);
int32_t  BMP085_ComputeB5(int32_t UT);

void     BMP085_Init(uint8_t mode);
float    BMP085_ReadTemperature(void);

uint16_t BMP085_ReadRawTemperature(void);
uint32_t BMP085_ReadRawPressure(void);
int32_t  BMP085_ReadPressure(void);

#endif // BMP085_H
