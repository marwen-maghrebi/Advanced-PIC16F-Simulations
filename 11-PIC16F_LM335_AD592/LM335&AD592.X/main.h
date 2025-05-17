
#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>


// Configuration Bits
#pragma config FOSC = XT       // Oscillator Selection bits (XT oscillator)
#pragma config WDTE = OFF      // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF     // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON      // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF       // Low-Voltage Programming Disable
#pragma config CPD = OFF       // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF       // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF        // Flash Program Memory Code Protection bit (Code protection off)

#define _XTAL_FREQ 4000000  // Replace with your clock frequency

#endif /* MAIN_H */



