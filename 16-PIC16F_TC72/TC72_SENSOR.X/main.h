
#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

// Configuration Bits
#pragma config FOSC = XT       // Oscillator Selection bits XT
#pragma config WDTE = OFF      // Watchdog Timer disabled bit WDT
#pragma config PWRTE = OFF     // Power-up Timer disabled bit PWRT
#pragma config BOREN = ON      // Brown-out Reset Enable bit BOR
#pragma config LVP = OFF       // Low-Voltage Programming Disable
#pragma config CPD = OFF       // Data EEPROM Memory Code Protection bit  off
#pragma config WRT = OFF       // Flash Program Memory Write Enable bits  off
#pragma config CP = OFF        // Flash Program Memory Code Protection bit off

#define _XTAL_FREQ 16000000

#endif /* MAIN_H */

