
#ifndef HYT_271_H
#define HYT_271_H

#include <xc.h>
#include "i2c.h"
// Sensor I2C Address
#define SLAVE_ADDR 0x28

// Function Declarations
void HYT271_Init(void);             // Initialize the sensor
void HYT271_StartMeasurement(void); // Start measurement on the sensor
void HYT271_ReadData(void);         // Read data from the sensor
float HYT271_GetTemperature(void);  // Get the measured temperature
float HYT271_GetHumidity(void);     // Get the measured humidity

#endif  // HYT_271_H
