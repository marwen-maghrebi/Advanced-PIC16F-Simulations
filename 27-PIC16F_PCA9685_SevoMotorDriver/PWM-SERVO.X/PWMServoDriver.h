
#ifndef _PWMSERVODRIVER_H
#define _PWMSERVODRIVER_H

#include "i2c.h"

#define PCA9685_MODE1 0x00
#define PCA9685_MODE2 0x01
#define PCA9685_SUBADR1 0x02
#define PCA9685_SUBADR2 0x03
#define PCA9685_SUBADR3 0x04
#define PCA9685_ALLCALLADR 0x05
#define PCA9685_LED0_ON_L 0x06
#define PCA9685_LED0_ON_H 0x07
#define PCA9685_LED0_OFF_L 0x08
#define PCA9685_LED0_OFF_H 0x09
#define PCA9685_ALLLED_ON_L 0xFA
#define PCA9685_ALLLED_ON_H 0xFB
#define PCA9685_ALLLED_OFF_L 0xFC
#define PCA9685_ALLLED_OFF_H 0xFD
#define PCA9685_PRESCALE 0xFE
#define PCA9685_TESTMODE 0xFF

#define MODE1_ALLCAL 0x01
#define MODE1_SUB3 0x02
#define MODE1_SUB2 0x04
#define MODE1_SUB1 0x08
#define MODE1_SLEEP 0x10
#define MODE1_AI 0x20
#define MODE1_EXTCLK 0x40
#define MODE1_RESTART 0x80

#define MODE2_OUTNE_0 0x01
#define MODE2_OUTNE_1 0x02
#define MODE2_OUTDRV 0x04
#define MODE2_OCH 0x08
#define MODE2_INVRT 0x10

#define PCA9685_I2C_ADDRESS 0x40
#define FREQUENCY_OSCILLATOR 25000000

#define PCA9685_PRESCALE_MIN 3
#define PCA9685_PRESCALE_MAX 255

void PWMServoDriver_Init(uint8_t addr, uint8_t prescale);
void PWMServoDriver_Reset(void);
void PWMServoDriver_Sleep(void);
void PWMServoDriver_Wakeup(void);
void PWMServoDriver_SetExtClk(uint8_t prescale);
void PWMServoDriver_SetPWMFreq(float freq);
void PWMServoDriver_SetOutputMode(uint8_t totempole);
uint16_t PWMServoDriver_GetPWM(uint8_t num, uint8_t off);
void PWMServoDriver_SetPWM(uint8_t num, uint16_t on, uint16_t off);
void PWMServoDriver_SetPin(uint8_t num, uint16_t val, uint8_t invert);
uint8_t PWMServoDriver_ReadPrescale(void);
void PWMServoDriver_WriteMicroseconds(uint8_t num, uint16_t microseconds);
void PWMServoDriver_SetOscillatorFrequency(uint32_t freq);
uint32_t PWMServoDriver_GetOscillatorFrequency(void);
uint8_t PWMServoDriver_Read8(uint8_t addr);
void PWMServoDriver_Write8(uint8_t addr, uint8_t data);

#endif /* _PWMSERVODRIVER_H */

