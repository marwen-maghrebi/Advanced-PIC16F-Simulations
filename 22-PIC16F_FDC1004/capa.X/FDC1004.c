
#include "FDC1004.h"

#define FDC1004_UPPER_BOUND ((int16_t) 0x4000)
#define FDC1004_LOWER_BOUND (-1 * FDC1004_UPPER_BOUND)

static uint8_t MEAS_CONFIG[] = {0x08, 0x09, 0x0A, 0x0B};
static uint8_t MEAS_MSB[] = {0x00, 0x02, 0x04, 0x06};
static uint8_t MEAS_LSB[] = {0x01, 0x03, 0x05, 0x07};

void FDC1004_Init(FDC1004 *fdc, uint8_t rate) {
    fdc->address = 0x50; // Default I2C address for FDC1004
    fdc->rate = rate;
    for (int i = 0; i < 4; i++) {
        fdc->last_capdac[i] = 0;
        }
    }

void FDC1004_Write16(FDC1004 *fdc, uint8_t reg, uint16_t data) {
    I2C_Master_Start();
    I2C_Master_Write((fdc->address << 1)); // Address + write bit
    I2C_Master_Write(reg); // Register
    I2C_Master_Write((data >> 8)); // MSB
    I2C_Master_Write(data & 0xFF); // LSB
    I2C_Master_Stop();
    }

uint16_t FDC1004_Read16(FDC1004 *fdc, uint8_t reg) {
    uint16_t value = 0;
    I2C_Master_Start();
    I2C_Master_Write((fdc->address << 1)); // Address + write bit
    I2C_Master_Write(reg); // Register
    I2C_Master_Stop();
    
    I2C_Master_Start();
    I2C_Master_Write((fdc->address << 1) | 1); // Address + read bit
    value = I2C_Read_Byte();
    I2C_ACK();
    value = (value << 8) | I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    return value;
    }

uint8_t FDC1004_ConfigureMeasurement(FDC1004 *fdc, uint8_t measurement, uint8_t channel, uint8_t capdac) {
    if (!FDC1004_IS_MEAS(measurement) || !FDC1004_IS_CHANNEL(channel) || capdac > FDC1004_CAPDAC_MAX) {
        return 1;
        }
    uint16_t config = ((uint16_t)channel << 13) | (0x04 << 10) | (capdac << 5);
    FDC1004_Write16(fdc, MEAS_CONFIG[measurement], config);
    return 0;
    }

uint8_t FDC1004_TriggerMeasurement(FDC1004 *fdc, uint8_t measurement, uint8_t rate) {
    if (!FDC1004_IS_MEAS(measurement) || !FDC1004_IS_RATE(rate)) {
        return 1;
        }
    uint16_t trigger = (rate << 10) | (1 << (7 - measurement));
    FDC1004_Write16(fdc, FDC_REGISTER, trigger);
    return 0;
    }

uint8_t FDC1004_ReadMeasurement(FDC1004 *fdc, uint8_t measurement, uint16_t *value) {
    if (!FDC1004_IS_MEAS(measurement)) {
        return 1;
        }
    uint16_t fdc_register = FDC1004_Read16(fdc, FDC_REGISTER);
    if (!(fdc_register & (1 << (3 - measurement)))) {
        return 2; // Measurement not ready
        }
    value[0] = FDC1004_Read16(fdc, MEAS_MSB[measurement]);
    value[1] = FDC1004_Read16(fdc, MEAS_LSB[measurement]);
    return 0;
    }

uint8_t FDC1004_MeasureChannel(FDC1004 *fdc, uint8_t channel, uint8_t capdac, uint16_t *value) {
    uint8_t measurement = channel;
    if (FDC1004_ConfigureMeasurement(fdc, measurement, channel, capdac)) return 1;
    if (FDC1004_TriggerMeasurement(fdc, measurement, fdc->rate)) return 1;
    __delay_ms(10); // Delay for measurement
    return FDC1004_ReadMeasurement(fdc, measurement, value);
    }

int32_t FDC1004_GetCapacitance(FDC1004 *fdc, uint8_t channel) {
    fdc1004_measurement_t result;
    if (FDC1004_GetRawCapacitance(fdc, channel, &result)) return INT32_MIN;
    int32_t capacitance = (int32_t)result.value * ATTOFARADS_UPPER_WORD / 1000;
    capacitance += (int32_t)result.capdac * FEMTOFARADS_CAPDAC;
    return capacitance;
    }

uint8_t FDC1004_GetRawCapacitance(FDC1004 *fdc, uint8_t channel, fdc1004_measurement_t *result) {
    if (!FDC1004_IS_CHANNEL(channel)) return 1;
    uint16_t raw_value[2];
    result->capdac = fdc->last_capdac[channel];
    result->value = 0x7FFF;
    
    while (result->value > 0x7E00 || result->value < 0x8100) {
        if (FDC1004_MeasureChannel(fdc, channel, result->capdac, raw_value)) return 1;
        result->value = (int16_t)raw_value[0];
        if (result->value > FDC1004_UPPER_BOUND && result->capdac < FDC1004_CAPDAC_MAX) {
            result->capdac++;
            } else if (result->value < FDC1004_LOWER_BOUND && result->capdac > 0) {
                result->capdac--;
                } else {
                fdc->last_capdac[channel] = result->capdac;
                return 0;
                }
        }
    fdc->last_capdac[channel] = result->capdac;
    return 0;
    }

