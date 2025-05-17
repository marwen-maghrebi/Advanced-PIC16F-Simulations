
// LM75.h
#ifndef LM75_H
#define LM75_H
#include "i2c.h"

// Device Address
#define LM75_ADDRESS           0x90      // LM75 I2C address

// Register Addresses
#define TEMPERATURE_ADDRESS    0x00      // Temperature register
#define CONFIGURATION_ADDRESS  0x01      // Configuration register
#define THYST_ADDRESS          0x02      // Hysteresis register
#define TOS_ADDRESS            0x03      // Over-temperature shutdown threshold

// Configuration Register Bit Masks
#define SHUTDOWN_MODE         0x01      // D0: Shutdown bit
#define INTERRUPT_MODE        0x02      // D1: Interrupt/Comparator mode bit
#define OS_POLARITY           0x04      // D2: O.S. Polarity bit
#define FAULT_QUEUE_MASK      0x18      // D3-D4: Fault Queue bits

// Fault Queue Values
#define FAULT_QUEUE_1        0x00      // 1 fault (default)
#define FAULT_QUEUE_2        0x08      // 2 faults
#define FAULT_QUEUE_4        0x10      // 4 faults
#define FAULT_QUEUE_6        0x18      // 6 faults

#define FRACTION_FLAG        0x0080    // 0.5-degree flag in temperature register

// Function declarations
void  LM75_init(void);
float LM75_read(char regAddress);
void  LM75_setConfig(char data);
char  LM75_readConfig(void);
void  LM75_set(char regAddress, signed int data);
float toFloat(signed int tempr);

// New function declarations
void LM75_setShutdownMode(char enable);
void LM75_setDeviceMode(char interruptMode);
void LM75_setOSPolarity(char activeHigh);
void LM75_setFaultQueue(char faultCount);
char LM75_isShutdown(void);
char LM75_getDeviceMode(void);
char LM75_getOSPolarity(void);
char LM75_getFaultQueue(void);

#ifndef LM75A_INVALID_TEMPERATURE
#define LM75A_INVALID_TEMPERATURE -1000.0f  // Arbitrary value outside sensor limits
#endif

float LM75_getHysteresisTemperature(void);
float LM75_getOSTripTemperature(void);
void LM75_setHysteresisTemperature(float temperature);
void LM75_setOSTripTemperature(float temperature);

#endif  // LM75_H


