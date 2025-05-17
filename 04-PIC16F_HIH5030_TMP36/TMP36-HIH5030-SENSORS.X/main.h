
#ifndef MAIN_H
#define MAIN_H

#include <xc.h>
#include "lcd.h"

// Define crystal frequency for delay calculations
#define _XTAL_FREQ 20000000

// Configuration bits
#pragma config FOSC = HS     // High-speed oscillator
#pragma config WDTE = OFF    // Watchdog Timer disabled
#pragma config PWRTE = OFF   // Power-up Timer disabled
#pragma config BOREN = OFF   // Brown-out Reset disabled
#pragma config LVP = ON      // Low-Voltage Programming enabled
#pragma config CPD = OFF     // Data EEPROM Code Protection disabled
#pragma config WRT = OFF     // Flash Program Memory Write Protection disabled
#pragma config CP = OFF      // Flash Program Memory Code Protection disabled

#endif /* MAIN_H */


