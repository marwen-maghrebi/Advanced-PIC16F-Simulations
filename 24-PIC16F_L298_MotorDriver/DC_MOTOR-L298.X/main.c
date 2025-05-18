
#include <xc.h>
#include <stdint.h>

// Configuration Bits
#pragma config FOSC = XT        // XT Oscillator
#pragma config WDTE = OFF       // Watchdog Timer Disabled
#pragma config PWRTE = OFF      // Power-up Timer Disabled
#pragma config BOREN = ON       // Brown-out Reset Enabled
#pragma config LVP = OFF        // Low-Voltage Programming Disabled
#pragma config CPD = OFF        // Data EEPROM Code Protection Disabled
#pragma config WRT = OFF        // Flash Program Write Protection Disabled
#pragma config CP = OFF         // Flash Program Code Protection Disabled

#define _XTAL_FREQ 16000000  // 16 MHz crystal frequency

// Motor Control Pins
#define ENA RC2   // PWM for Motor A
#define IN1 RD0   // Direction for Motor A
#define IN2 RD1   // Direction for Motor A
#define ENB RC1   // PWM for Motor B
#define IN3 RD2   // Direction for Motor B
#define IN4 RD3   // Direction for Motor B

// Direction Definitions
#define FORWARD 1
#define BACKWARD 2

// Function Prototypes
void setupPWM();
void setPWMDutyCycle(int speedA, int speedB);
void runMotorA(int direction, int speed);
void runMotorB(int direction, int speed);
void stopMotors();
void delay_ms(unsigned int ms);

void main(void) {
    // Configure I/O Pins
    TRISC = 0x00; // PORTC as output (PWM)
    TRISD = 0x00; // PORTD as output (Direction)
    PORTC = 0x00; // Clear PORTC
    PORTD = 0x00; // Clear PORTD
    
    // Initialize PWM
    setupPWM();
    
    while (1) {
        // Run Motor A Backward and Motor B Forward at full speed
        runMotorA(BACKWARD, 255); // 100% duty cycle for Motor A
        runMotorB(FORWARD, 255); // 100% duty cycle for Motor B
        delay_ms(8000); // Run for 8 seconds
        
        // Stop Motors
        stopMotors();
        delay_ms(8000); // Pause for 8 seconds
        
        // Run Motor A Forward and Motor B Backward  at full speed
        runMotorA(FORWARD, 255); // 100% duty cycle for Motor A
        runMotorB(BACKWARD, 255); // 100% duty cycle for Motor B
        delay_ms(8000); // Run for 8 seconds
        
        // Stop Motors
        stopMotors();
        delay_ms(8000); // Pause for 8 seconds
    }
}

// Function to configure Timer2 for PWM generation
void setupPWM() {
    T2CON = 0xFF;   // Timer2 ON, prescaler = 1:16, postscaler = 1:16
    PR2 = 255;      // Set PR2 for Timer2 maximum count
    CCP1CON = 0x0C; // Set CCP1 to PWM mode (Motor A)
    CCP2CON = 0x0C; // Set CCP2 to PWM mode (Motor B)
    CCPR1L = 0;     // Initialize PWM duty cycle for CCP1
    CCPR2L = 0;     // Initialize PWM duty cycle for CCP2
}

// Function to set PWM duty cycles for both motors
void setPWMDutyCycle(int speedA, int speedB) {
    CCPR1L = (unsigned char)speedA; // Set PWM duty cycle for Motor A
    CCPR2L = (unsigned char)speedB; // Set PWM duty cycle for Motor B
}

// Function to run Motor A
void runMotorA(int direction, int speed) {
    setPWMDutyCycle(speed, CCPR2L); // Update PWM for Motor A
    if (direction == FORWARD) {
        IN1 = 1; // Forward direction
        IN2 = 0;
    } else if (direction == BACKWARD) {
        IN1 = 0; // Backward direction
        IN2 = 1;
    }
}

// Function to run Motor B
void runMotorB(int direction, int speed) {
    setPWMDutyCycle(CCPR1L, speed); // Update PWM for Motor B
    if (direction == FORWARD) {
        IN3 = 1; // Forward direction
        IN4 = 0;
    } else if (direction == BACKWARD) {
        IN3 = 0; // Backward direction
        IN4 = 1;
    }
}

// Function to stop both motors
void stopMotors() {
    CCPR1L = 0; // Stop Motor A
    CCPR2L = 0; // Stop Motor B
    IN1 = 0;    // Turn off Motor A
    IN2 = 0;
    IN3 = 0;    // Turn off Motor B
    IN4 = 0;
}

// Custom delay function (in milliseconds)
void delay_ms(unsigned int ms) {
    while (ms--) {
    __delay_ms(1); // XC8 built-in delay function
    }
}


