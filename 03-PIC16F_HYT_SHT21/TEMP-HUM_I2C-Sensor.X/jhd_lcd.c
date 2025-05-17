

#include "jhd_lcd.h"
#include "i2c.h"

static unsigned char _lcd_addr;
static unsigned char _displaycontrol;
static unsigned char _displaymode;
static unsigned char _initialized;
static unsigned char _numlines;
static unsigned char _currline;

static void LCD_Send(unsigned char data, unsigned char mode) {
    I2C_Master_Start();
    I2C_Master_Write(_lcd_addr);      // LCD I2C address
    I2C_Master_Write(mode);            // Mode: 0x80 for command, 0x40 for data
    I2C_Master_Write(data);            // Actual data or command to be sent
    I2C_Master_Stop();
    __delay_us(50);                    // Wait for LCD to process
    }

void LCD_Command(unsigned char cmd) {
    LCD_Send(cmd, 0x80);  // Send command mode
    }

void LCD_Print_Char(char c) {
    LCD_Send(c, 0x40);  // Send character data mode
    }

void LCD_Init(unsigned char lcd_addr) {
    _lcd_addr = lcd_addr;
    _initialized = 0;
    _displaycontrol = LCD_DISPLAYON | LCD_CURSOROFF | LCD_BLINKOFF;
    _displaymode = LCD_ENTRYLEFT | LCD_ENTRYSHIFTDEC;
    
    // Initialize display
    __delay_ms(50);  // Wait for LCD to stabilize
    
    // 4-bit initialization sequence
    LCD_Command(0x33);
    __delay_ms(5);
    LCD_Command(0x32);
    __delay_ms(5);
    
    // Function set: 4-bit mode, 2-line display, 5x8 dots
    LCD_Command(LCD_FUNCTIONSET | LCD_2LINE | LCD_5x8DOTS | LCD_4BITMODE);
    __delay_ms(5);
    
    // Display control
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
    __delay_ms(5);
    
    // Clear display
    LCD_Clear();
    
    // Entry mode set
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
    
    _initialized = 1;
    }

void LCD_Clear(void) {
    LCD_Command(LCD_CLEARDISPLAY);
    __delay_ms(2);  // Command takes longer
    }

void LCD_Home(void) {
    LCD_Command(LCD_RETURNHOME);
    __delay_ms(2);  // Command takes longer
    }

void LCD_Display_ON(void) {
    _displaycontrol |= LCD_DISPLAYON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

void LCD_Display_OFF(void) {
    _displaycontrol &= ~LCD_DISPLAYON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

void LCD_Cursor_ON(void) {
    _displaycontrol |= LCD_CURSORON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

void LCD_Cursor_OFF(void) {
    _displaycontrol &= ~LCD_CURSORON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

void LCD_Blink_ON(void) {
    _displaycontrol |= LCD_BLINKON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

void LCD_Blink_OFF(void) {
    _displaycontrol &= ~LCD_BLINKON;
    LCD_Command(LCD_DISPLAYCONTROL | _displaycontrol);
    }

void LCD_ScrollDisplayLeft(void) {
    LCD_Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVELEFT);
    }

void LCD_ScrollDisplayRight(void) {
    LCD_Command(LCD_CURSORSHIFT | LCD_DISPLAYMOVE | LCD_MOVERIGHT);
    }

void LCD_LeftToRight(void) {
    _displaymode |= LCD_ENTRYLEFT;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
    }

void LCD_RightToLeft(void) {
    _displaymode &= ~LCD_ENTRYLEFT;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
    }

void LCD_AutoScroll_ON(void) {
    _displaymode |= LCD_ENTRYSHIFTINC;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
    }

void LCD_AutoScroll_OFF(void) {
    _displaymode &= ~LCD_ENTRYSHIFTINC;
    LCD_Command(LCD_ENTRYMODESET | _displaymode);
    }

void LCD_CreateCustomChar(unsigned char location, unsigned char* charmap) {
    location &= 0x7;  // Only locations 0-7 are valid
    LCD_Command(LCD_SETCGRAMADDR | (location << 3));
    
    for (unsigned char i = 0; i < 8; i++) {
        LCD_Send(charmap[i], 0x40);  // Data mode
        }
    }

void LCD_SetCursor(unsigned char col, unsigned char row) {
    unsigned char row_offsets[] = {0x80, 0xC0, 0x94, 0xD4};
    
    if (row >= 4) {
        row = 0;  // Default to first row if invalid
        }
    
    LCD_Command(row_offsets[row] + col);
    }

void LCD_Print_String(const char* str) {
    while (*str) {
        LCD_Print_Char(*str++);
        }
    }
