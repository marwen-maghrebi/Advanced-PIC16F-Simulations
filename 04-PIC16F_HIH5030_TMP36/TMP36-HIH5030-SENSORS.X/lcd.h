#ifndef LCD_H
#define LCD_H

#include "main.h"

#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN PORTCbits.RC2

// Function prototypes
void lcd_initialize(void);
void lcd_data(unsigned char data);
void lcd_string(const unsigned char *str, unsigned char len);
void lcd_command(unsigned char cmd);

#endif /* LCD_H */