

#ifndef LCD_H
#define LCD_H

#include"i2c.h"

// Define LCD control pins (adjust these based on your hardware setup)
#define RS PORTCbits.RC0  // Register Select pin
#define RW PORTCbits.RC1  // Read/Write pin
#define EN PORTCbits.RC2  // Enable pin

// Function Prototypes
void lcd_initialize(void);
void lcd_data(unsigned char data);
void lcd_string(const unsigned char *str, unsigned char len);
void lcd_command(unsigned char cmd);

#endif /* LCD_H */

