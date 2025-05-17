

#ifndef FDC1004_H
#define FDC1004_H
#include "i2c.h"

#include <xc.h>
#include <stdint.h>

// Constants and limits for FDC1004
#define FDC1004_100HZ (0x01)
#define FDC1004_200HZ (0x02)
#define FDC1004_400HZ (0x03)
#define FDC1004_IS_RATE(x) (x == FDC1004_100HZ ||   x == FDC1004_200HZ ||   x == FDC1004_400HZ)

#define FDC1004_CAPDAC_MAX (0x1F)

#define FDC1004_CHANNEL_MAX (0x03)
#define FDC1004_IS_CHANNEL(x) (x >= 0 && x <= FDC1004_CHANNEL_MAX)

#define FDC1004_MEAS_MAX (0x03)
#define FDC1004_IS_MEAS(x) (x >= 0 && x <= FDC1004_MEAS_MAX)

#define FDC_REGISTER (0x0C)

// Measurement scaling constants
#define ATTOFARADS_UPPER_WORD (488)
#define FEMTOFARADS_CAPDAC (3125)

typedef struct {
    int16_t value;
    uint8_t capdac;
    } fdc1004_measurement_t;
    
    typedef struct {
        uint8_t address;
        uint8_t rate;
        uint8_t last_capdac[4];
        } FDC1004;
        
        // Function prototypes
        void FDC1004_Init(FDC1004 *fdc, uint8_t rate);
        uint8_t FDC1004_ConfigureMeasurement(FDC1004 *fdc, uint8_t measurement, uint8_t channel, uint8_t capdac);
        uint8_t FDC1004_TriggerMeasurement(FDC1004 *fdc, uint8_t measurement, uint8_t rate);
        uint8_t FDC1004_ReadMeasurement(FDC1004 *fdc, uint8_t measurement, uint16_t *value);
        uint8_t FDC1004_MeasureChannel(FDC1004 *fdc, uint8_t channel, uint8_t capdac, uint16_t *value);
        int32_t FDC1004_GetCapacitance(FDC1004 *fdc, uint8_t channel);
        uint8_t FDC1004_GetRawCapacitance(FDC1004 *fdc, uint8_t channel, fdc1004_measurement_t *value);
        
#endif /* FDC1004_H */
        
