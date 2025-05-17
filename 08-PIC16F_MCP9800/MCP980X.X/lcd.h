
#ifndef LCD_H
#define LCD_H
#include "i2c.h"
#define Line0               0x80
#define Line1               0xC0
void initializeLCD(void);
void clearLCD(void);
void sendCommandToLCD(unsigned char command);
void sendDataToLCD(unsigned char data);
void printToLCD(const char *string);
unsigned char moveCursorToPosition(unsigned char address);

#endif // LCD_H
