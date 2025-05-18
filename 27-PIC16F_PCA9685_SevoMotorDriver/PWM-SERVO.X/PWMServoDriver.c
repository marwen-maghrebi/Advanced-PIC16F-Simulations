
#include "PWMServoDriver.h"

static uint8_t _i2caddr = PCA9685_I2C_ADDRESS;
static uint32_t _oscillator_freq = FREQUENCY_OSCILLATOR;

void PWMServoDriver_Init(uint8_t addr, uint8_t prescale) {
    _i2caddr = addr;
    I2C_Master_Init();
    PWMServoDriver_Reset();
    PWMServoDriver_SetOscillatorFrequency(FREQUENCY_OSCILLATOR);
    if (prescale) {
        PWMServoDriver_SetExtClk(prescale);
        } else {
        PWMServoDriver_SetPWMFreq(1000);
        }
    }

void PWMServoDriver_Reset(void) {
    PWMServoDriver_Write8(PCA9685_MODE1, MODE1_RESTART);
    __delay_ms(10);
    }

void PWMServoDriver_Sleep(void) {
    uint8_t mode1 = PWMServoDriver_Read8(PCA9685_MODE1);
    mode1 |= MODE1_SLEEP;
    PWMServoDriver_Write8(PCA9685_MODE1, mode1);
    __delay_ms(5);
    }

void PWMServoDriver_Wakeup(void) {
    uint8_t mode1 = PWMServoDriver_Read8(PCA9685_MODE1);
    mode1 &= ~MODE1_SLEEP;
    PWMServoDriver_Write8(PCA9685_MODE1, mode1);
    }

void PWMServoDriver_SetExtClk(uint8_t prescale) {
    uint8_t oldmode = PWMServoDriver_Read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP;
    PWMServoDriver_Write8(PCA9685_MODE1, newmode);
    PWMServoDriver_Write8(PCA9685_MODE1, newmode | MODE1_EXTCLK);
    PWMServoDriver_Write8(PCA9685_PRESCALE, prescale);
    __delay_ms(5);
    PWMServoDriver_Write8(PCA9685_MODE1, (newmode & ~MODE1_SLEEP) | MODE1_RESTART | MODE1_AI);
    }

void PWMServoDriver_SetPWMFreq(float freq) {
    if (freq < 1) freq = 1;
    if (freq > 3500) freq = 3500;
    float prescaleval = ((_oscillator_freq / (freq * 4096.0)) + 0.5) - 1;
    if (prescaleval < PCA9685_PRESCALE_MIN) prescaleval = PCA9685_PRESCALE_MIN;
    if (prescaleval > PCA9685_PRESCALE_MAX) prescaleval = PCA9685_PRESCALE_MAX;
    uint8_t prescale = (uint8_t)prescaleval;
    uint8_t oldmode = PWMServoDriver_Read8(PCA9685_MODE1);
    uint8_t newmode = (oldmode & ~MODE1_RESTART) | MODE1_SLEEP;
    PWMServoDriver_Write8(PCA9685_MODE1, newmode);
    PWMServoDriver_Write8(PCA9685_PRESCALE, prescale);
    PWMServoDriver_Write8(PCA9685_MODE1, oldmode);
    __delay_ms(5);
    PWMServoDriver_Write8(PCA9685_MODE1, oldmode | MODE1_RESTART | MODE1_AI);
    }

void PWMServoDriver_SetOutputMode(uint8_t totempole) {
    uint8_t oldmode = PWMServoDriver_Read8(PCA9685_MODE2);
    uint8_t newmode;
    if (totempole) {
        newmode = oldmode | MODE2_OUTDRV;
        } else {
        newmode = oldmode & ~MODE2_OUTDRV;
        }
    PWMServoDriver_Write8(PCA9685_MODE2, newmode);
    }

uint16_t PWMServoDriver_GetPWM(uint8_t num, uint8_t off) {
    uint8_t reg = PCA9685_LED0_ON_L + 4 * num;
    if (off) {
        reg += 2;
        }
    uint8_t buffer[2];
    I2C_Master_Start();
    I2C_Master_Write((_i2caddr << 1) | 0);
    I2C_Master_Write(reg);
    I2C_Master_RepeatedStart();
    I2C_Master_Write((_i2caddr << 1) | 1);
    buffer[0] = I2C_Read_Byte();
    I2C_ACK();
    buffer[1] = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    return ((uint16_t)buffer[1] << 8) | buffer[0];
    }

void PWMServoDriver_SetPWM(uint8_t num, uint16_t on, uint16_t off) {
    uint8_t reg = PCA9685_LED0_ON_L + 4 * num;
    PWMServoDriver_Write8(reg, on & 0xFF);
    PWMServoDriver_Write8(reg + 1, on >> 8);
    PWMServoDriver_Write8(reg + 2, off & 0xFF);
    PWMServoDriver_Write8(reg + 3, off >> 8);
    }

void PWMServoDriver_SetPin(uint8_t num, uint16_t val, uint8_t invert) {
    if (val > 4095) {
        val = 4095;
        }
    if (invert) {
        if (val == 0) {
            PWMServoDriver_SetPWM(num, 4096, 0);
            } else if (val == 4095) {
                PWMServoDriver_SetPWM(num, 0, 4096);
                } else {
                PWMServoDriver_SetPWM(num, 0, 4095 - val);
                }
        } else {
        if (val == 4095) {
            PWMServoDriver_SetPWM(num, 4096, 0);
            } else if (val == 0) {
                PWMServoDriver_SetPWM(num, 0, 4096);
                } else {
                PWMServoDriver_SetPWM(num, 0, val);
                }
        }
    }

uint8_t PWMServoDriver_ReadPrescale(void) {
    return PWMServoDriver_Read8(PCA9685_PRESCALE);
    }

void PWMServoDriver_WriteMicroseconds(uint8_t num, uint16_t Microseconds) {
    double pulse = Microseconds;
    double pulselength = 1000000;
    uint16_t prescale = PWMServoDriver_ReadPrescale();
    prescale += 1;
    pulselength *= prescale;
    pulselength /= _oscillator_freq;
    pulse /= pulselength;
    PWMServoDriver_SetPWM(num, 0, (uint16_t)pulse);
    }

void PWMServoDriver_SetOscillatorFrequency(uint32_t freq) {
    _oscillator_freq = freq;
    }

uint32_t PWMServoDriver_GetOscillatorFrequency(void) {
    return _oscillator_freq;
    }

uint8_t PWMServoDriver_Read8(uint8_t addr) {
    I2C_Master_Start();
    I2C_Master_Write((_i2caddr << 1) | 0);
    I2C_Master_Write(addr);
    I2C_Master_RepeatedStart();
    I2C_Master_Write((_i2caddr << 1) | 1);
    uint8_t data = I2C_Read_Byte();
    I2C_NACK();
    I2C_Master_Stop();
    return data;
    }

void PWMServoDriver_Write8(uint8_t addr, uint8_t data) {
    I2C_Master_Start();
    I2C_Master_Write((_i2caddr << 1) | 0);
    I2C_Master_Write(addr);
    I2C_Master_Write(data);
    I2C_Master_Stop();
    }


