#ifndef TCN75A_H
#define TCN75A_H

#include <xc.h>
#include "i2c.h"

// TCN75A Register Addresses
#define TCN75A_TEMP_REG      0x00
#define TCN75A_CONFIG_REG    0x01
#define TCN75A_THYST_REG     0x02
#define TCN75A_TLIMIT_REG    0x03

// Configuration Options
#define ONESHOT              0x05
#define ADC_RESO             0x04
#define F_QUEUE              0x03
#define ALERT_POL            0x02
#define COMP_INT             0x01
#define SHUTDOWN             0x00

#define COMP_MODE            0
#define INT_MODE             1

// Function Prototypes
void TCN75A_Init(uint8_t adr);
float TCN75A_ReadTemperature();
void TCN75A_SetHystTemp(float val);
void TCN75A_SetLimitTemp(float val);
void TCN75A_SetRangeTemp(float val_down, float val_up);
float TCN75A_GetLimitTemp();
float TCN75A_GetHystTemp();
void TCN75A_SetOneShot(uint8_t sw);
void TCN75A_SetResolution(uint8_t val);
void TCN75A_SetFaultQueue(uint8_t val);
void TCN75A_SetAlertPolarity(uint8_t sw);
void TCN75A_SetAlertMode(uint8_t sw);
void TCN75A_SetShutdown(uint8_t sw);
int8_t TCN75A_CheckConfig(uint8_t op);
void TCN75A_SetTemp(uint8_t reg, float value);
float TCN75A_GetTemp(uint8_t reg);
uint8_t TCN75A_CheckAlert(void);

#endif

