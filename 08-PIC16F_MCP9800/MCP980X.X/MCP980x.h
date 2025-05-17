
#ifndef MCP980x_H
#define MCP980x_H
#include "i2c.h"
#include <stdint.h>
#include <stdbool.h>

// MCP9800 constants
#define MCP9800_ADDRESS     0x48
#define TEMP_REGISTER       0x00
#define CONFIG_REG          0x01
#define REG_HYSTERESIS      0x02
#define REG_LIMIT           0x03
#define TEMPERATURE_ERROR  -9990  // Error value

// ADC Resolution Values
#define ADC_RES_9BIT   0  // 9-bit resolution  (0.5C)
#define ADC_RES_10BIT  1  // 10-bit resolution (0.25C)
#define ADC_RES_11BIT  2  // 11-bit resolution (0.125C)
#define ADC_RES_12BIT  3  // 12-bit resolution (0.0625C)

// Alert Mode
#define MCP_COMPARATOR  0  // Alert pin active when temperature > alert value
#define MCP_INTERRUPT   1  // Alert pin asserted and reset by a microcontroller

// Fault Queue Types
#define MCP_1_FAULT     0  // Allow 1 fault before alert
#define MCP_2_FAULTS    1  // Allow 2 faults before alert
#define MCP_4_FAULTS    2  // Allow 4 faults before alert
#define MCP_6_FAULTS    3  // Allow 6 faults before alert

enum configuration {
    CONFIG_SHUTDOWN     = 0, // Bit 0 shutdown mode
    CONFIG_COMP_INT     = 1, // Bit 1 comparator/interrupt mode
    CONFIG_ALERT_POL    = 2, // Bit 2 alert polarity
    CONFIG_FAULT_QUEUE  = 3, // Bits 3-4 fault queu
    CONFIG_ADC_RES      = 5, // Bits 5-6 ADC resolution
    CONFIG_ONE_SHOT     = 7  // Bit 7 one shot mode
};

// Function prototypes
void MCP9800_Init(void);
void MCP9800_SetConfigOneShot(uint8_t enable);
void MCP9800_SetConfigResolution(uint8_t resolution);
void MCP9800_SetConfigFaultQueue(uint8_t fault_queue);
void MCP9800_SetConfigAlertPolarity(uint8_t polarity);
void MCP9800_SetConfigCompIntMode(uint8_t mode);
void MCP9800_SetConfigShutdown(uint8_t enable);
void MCP9800_WriteConfig(uint8_t value);
uint8_t MCP9800_ReadConfig(void);

uint8_t MCP9800_GetConfigOneShot(void);
uint8_t MCP9800_GetConfigResolution(void);
uint8_t MCP9800_GetConfigFaultQueue(void) ;
uint8_t MCP9800_GetConfigAlertPolarity(void);
uint8_t MCP9800_GetConfigCompIntMode(void);
uint8_t MCP9800_GetConfigShutdown(void);

int16_t MCP9800_ReadTemperature(void);
void MCP9800_SetHYST_Temp(int16_t temperature);
int16_t MCP9800_GetHYST_Temp(void) ;
void MCP9800_SetLIMIT_Temp(int16_t temperature) ;
int16_t MCP9800_GetLIMIT_Temp(void) ;
#endif // MCP9800_H
                
