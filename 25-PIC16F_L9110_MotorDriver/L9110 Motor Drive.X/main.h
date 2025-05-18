
// Project: PIC16F877A L9110 Motor Control Library
// Header File: main.h
// Overview: This header file defines configuration bits, constants, and function prototypes for motor control.

#ifndef MAIN_H
#define MAIN_H

#include <xc.h>

// Configuration Bits
#pragma config FOSC = HS        // High-Speed Oscillator
#pragma config WDTE = OFF       // Watchdog Timer Disabled
#pragma config PWRTE = ON       // Power-Up Timer Enabled
#pragma config BOREN = ON       // Brown-Out Reset Enabled
#pragma config LVP = OFF        // Low-Voltage Programming Disabled
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection Disabled
#pragma config WRT = OFF        // Flash Program Memory Write Protection Disabled
#pragma config CP = OFF         // Flash Program Memory Code Protection Disabled

#define _XTAL_FREQ 16000000     // Oscillator Frequency (16MHz)

#endif /* MAIN_H */
