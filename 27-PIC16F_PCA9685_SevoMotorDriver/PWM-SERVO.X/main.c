
#include "i2c.h"
#include "PWMServoDriver.h"

// Define servo pulse width boundaries
#define SERVOMIN  150 // Minimum pulse length count (out of 4096)
#define SERVOMAX  600 // Maximum pulse length count (out of 4096)

// Servo number counter
uint8_t servonum = 0;

void setServoPulse(uint8_t n, double pulse) {
    double pulselength;
    
    pulselength = 1000000;  // 1,000,000 us per second
    pulselength /= 60;      // 60 Hz
    pulselength /= 4096;    // 12 bits of resolution
    
    pulse *= 1000;          // Convert seconds to microseconds
    pulse /= pulselength;   // Convert microseconds to pulse width count
    
    PWMServoDriver_SetPWM(n, 0, (uint16_t)pulse);
    }

int main(void) {
    // Initialize I2C and PWM Servo Driver
    I2C_Master_Init();
    PWMServoDriver_Init(PCA9685_I2C_ADDRESS, 0); // Default I2C address 0x40
    
    PWMServoDriver_SetPWMFreq(60); // Set frequency to ~60 Hz
    
    while (1) {
        // Drive each servo one at a time
        for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {
            PWMServoDriver_SetPWM(servonum, 0, pulselen);
            }
        __delay_ms(500);
        
        for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
            PWMServoDriver_SetPWM(servonum, 0, pulselen);
            }
        __delay_ms(500);
        
        // Move to the next servo
        servonum++;
        if (servonum > 4) {
            servonum = 0;
            }
        }
    
    return 0;
    }



