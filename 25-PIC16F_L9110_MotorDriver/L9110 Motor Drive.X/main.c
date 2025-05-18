
// Project: PIC16F877A L9110 Motor Control Library
// Overview: This code implements a library-like structure to control multiple motors using the L9110 driver and UART for remote control.

#include <xc.h>
#include "main.h"
#include "uart.h"

// Function Prototypes
void Motor_Init(void);
void Motor_Control(int motor, int cmd, int speed);
void Motor_Forward(int speed);
void Motor_Backward(int speed);
void Motor_Right(int speed);
void Motor_Left(int speed);
void Motor_Stop(void);

// Global Variables for Motor Pins
unsigned char MOTOR1_IN1 = 0;
unsigned char MOTOR1_IN2 = 1;
unsigned char MOTOR2_IN1 = 2;
unsigned char MOTOR2_IN2 = 3;

void main(void) {
// Initialize the motor pins and UART
Motor_Init();
UART_TX_Init();
UART_Write_Text("L9110s_Diagnostic READY !\n\r");
UART_Write_Text("1 --> Forward, 2 --> Backward, 3 --> Left, 4 --> Right, 5 --> STOP, 6 --> Help\n\r");

while (1) {
    if (RCIF) { // Check if UART data is available
        char input = RCREG; // Read the received character

        switch (input) {
            case '1':
            Motor_Forward(255); // Full speed forward
            UART_Write_Text("Forward\n\r");
            break;
            case '2':
            Motor_Backward(255); // Full speed backward
            UART_Write_Text("Backward\n\r");
            break;
            case '3':
            Motor_Left(255); // Full speed left
            UART_Write_Text("Left\n\r");
            break;
            case '4':
            Motor_Right(255); // Full speed right
            UART_Write_Text("Right\n\r");
            break;
            case '5':
            Motor_Stop(); // Stop the motors
            UART_Write_Text("Stopped\n\r");
            break;
            case '6':
            UART_Write_Text("1 --> Forward, 2 --> Backward, 3 --> Left, 4 --> Right, 5 --> STOP, 6 --> Help\n\r");
            break;
            default:
            UART_Write_Text("Invalid Command\n\r");
            break;
            }
        __delay_ms(500); // Debounce delay
        }
    }
}

// Initialize the motor control pins
void Motor_Init(void) {
    TRISBbits.TRISB0 = 0;
    TRISBbits.TRISB1 = 0;
    TRISBbits.TRISB2 = 0;
    TRISBbits.TRISB3 = 0;
    PORTB = 0; // Ensure all motor control pins are low initially
    }

// Control individual motors
void Motor_Control(int motor, int cmd, int speed) {
    unsigned char in1, in2;
    
    if (motor == 1) {
    in1 = MOTOR1_IN1;
    in2 = MOTOR1_IN2;
    } else if (motor == 2) {
    in1 = MOTOR2_IN1;
    in2 = MOTOR2_IN2;
    } else {
    return; // Invalid motor
    }
    
    switch (cmd) {
        case 1: // Forward
        PORTB |= (1 << in1);
        PORTB &= ~(1 << in2);
        break;
        case 2: // Reverse
        PORTB &= ~(1 << in1);
        PORTB |= (1 << in2);
        break;
        case 3: // Stop
        PORTB &= ~(1 << in1);
        PORTB &= ~(1 << in2);
        break;
        default:
            break;
    }

    // Simulating speed control by PWM (simple delay-based approach for demonstration)
    if (speed > 0) {
    for (int i = 0; i < speed; i++) {
        __delay_us(100);
        }
    }
}

// Move forward
void Motor_Forward(int speed) {
    Motor_Control(1, 1, speed);
    Motor_Control(2, 1, speed);
    }

// Move backward
void Motor_Backward(int speed) {
    Motor_Control(1, 2, speed);
    Motor_Control(2, 2, speed);
    }

// Turn right
void Motor_Right(int speed) {
    Motor_Control(1, 1, speed);
    Motor_Control(2, 2, speed);
    }

// Turn left
void Motor_Left(int speed) {
    Motor_Control(1, 2, speed);
    Motor_Control(2, 1, speed);
    }

// Stop all motors
void Motor_Stop(void) {
    Motor_Control(1, 3, 0);
    Motor_Control(2, 3, 0);
    }

