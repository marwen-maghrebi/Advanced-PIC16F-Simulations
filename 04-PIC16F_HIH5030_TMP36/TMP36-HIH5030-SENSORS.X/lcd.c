#include "lcd.h"

void lcd_initialize(void) {
    lcd_command(0x02); // Return cursor to home position
    lcd_command(0x28); // Set 4-bit mode, 2-line display, 5x8 font
    lcd_command(0x06); // Set entry mode with auto-increment
    lcd_command(0x0C); // Display ON, cursor OFF
    lcd_command(0x01); // Clear display
    __delay_ms(2);     // Short delay for clear screen command
}

void lcd_data(unsigned char data) {
    RS = 1; RW = 0;       // Set to data mode and write
    PORTD = (data & 0xF0); // Send upper nibble
    EN = 1;
    __delay_ms(2);
    EN = 0;
    
    PORTD = ((data << 4) & 0xF0); // Send lower nibble
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_string(const unsigned char *str, unsigned char len) {
    for (unsigned char i = 0; i < len; ++i) {
        lcd_data(str[i]); // Send each character in the string
    }
}


void lcd_command(unsigned char cmd) {
    RS = 0; RW = 0;       // Set to command mode and write
    PORTD = (cmd & 0xF0); // Send upper nibble
    EN = 1;
    __delay_ms(2);
    EN = 0;
    
    PORTD = ((cmd << 4) & 0xF0); // Send lower nibble
    EN = 1;
    __delay_ms(2);
    EN = 0;
    }
