#ifndef SHT2X_H
#define SHT2X_H

#include <stdint.h>
#include <math.h>
#include "i2c.h"

// Constants for dew point calculation
#define WATER_VAPOR 17.62f
#define BAROMETRIC_PRESSURE 243.5f

// SHT2x sensor address and commands
#define SHT2x_ADDRESS       0x40
#define TEMP_HOLD_CMD       0xE3
#define RH_HOLD_CMD         0xE5
#define TEMP_NOHOLD_CMD     0xF3
#define RH_NOHOLD_CMD       0xF5

// Function prototypes
float SHT2x_GetHumidity(void);
float SHT2x_GetTemperature(void);
float SHT2x_GetDewPoint(void);

#endif


