
#ifndef MCP3421_H
#define MCP3421_H

#include "main.h"
#include "i2c.h"

// MCP3421 Configuration Constants
#define MCP3421_RDY_BIT   7
#define MCP3421_OC_BIT    4

// MCP3421 Struct for Storing Configuration
typedef struct {
    int address;     // I2C address
    uint8_t sr;      // Sample Rate
    uint8_t pga;     // PGA Gain
    uint8_t config;  // Configuration Byte
} MCP3421;

// Function Prototypes
void MCP3421_Init(MCP3421 *adc, int address, uint8_t sr, uint8_t pga);
long MCP3421_GetLong(MCP3421 *adc);
double MCP3421_GetDouble(MCP3421 *adc);
int MCP3421_Ready(MCP3421 *adc);
    
#endif // MCP3421_H
    
