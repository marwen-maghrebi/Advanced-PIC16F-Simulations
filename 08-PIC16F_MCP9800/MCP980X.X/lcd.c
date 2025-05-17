
#include "lcd.h"
#include <xc.h>

#define LCD_BUS             PORTB
#define ENABLE_LCD          PORTCbits.RC1
#define RS_LCD              PORTCbits.RC0

#define LCD_BUS_DIRECTION   TRISB
#define ENABLE_DIRECTION    TRISCbits.TRISC1
#define RS_DIRECTION        TRISCbits.TRISC0

#define Line0               0x80
#define Line1               0xC0

void initializeLCD(void) {
    ENABLE_DIRECTION = 0;
    RS_DIRECTION = 0;
    LCD_BUS_DIRECTION = 0;
    ENABLE_LCD = 0;
    RS_LCD = 0;
    LCD_BUS = 0;
    
    __delay_ms(10);
    sendCommandToLCD(0x38);
    sendCommandToLCD(0x0F);
    sendCommandToLCD(0x06);
    clearLCD();
    }

void clearLCD(void) {
    sendCommandToLCD(0x01);
    __delay_ms(3);
    }

void sendCommandToLCD(unsigned char command) {
    RS_LCD = 0;
    LCD_BUS = command;
    ENABLE_LCD = 1;
    __delay_us(200);
    ENABLE_LCD = 0;
    __delay_us(200);
    }

void sendDataToLCD(unsigned char data) {
    RS_LCD = 1;
    LCD_BUS = data;
    ENABLE_LCD = 1;
    __delay_us(200);
    ENABLE_LCD = 0;
    __delay_us(200);
    }

void printToLCD(const char *string) {
    while(*string) {
        sendDataToLCD(*string++);
        }
    }

unsigned char moveCursorToPosition(unsigned char address) {
    if ((address >= 0x80 && address <= 0xA8) || (address >= 0xC0 && address <= 0xE8)) {
        sendCommandToLCD(address);
        return 1;
        } else {
        return 0;
        }
    }

