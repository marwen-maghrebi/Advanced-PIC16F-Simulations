#ifndef JHD_LCD_H
#define JHD_LCD_H

#include <xc.h>

// LCD Commands
#define LCD_CLEARDISPLAY      0x01
#define LCD_RETURNHOME        0x02
#define LCD_ENTRYMODESET      0x04
#define LCD_DISPLAYCONTROL    0x08
#define LCD_CURSORSHIFT       0x10
#define LCD_FUNCTIONSET       0x20
#define LCD_SETCGRAMADDR      0x40
#define LCD_SETDDRAMADDR      0x80

// LCD Entry Mode
#define LCD_ENTRYRIGHT        0x00
#define LCD_ENTRYLEFT         0x02
#define LCD_ENTRYSHIFTINC     0x01
#define LCD_ENTRYSHIFTDEC     0x00

// LCD Display Control
#define LCD_DISPLAYON         0x04
#define LCD_DISPLAYOFF        0x00
#define LCD_CURSORON          0x02
#define LCD_CURSOROFF         0x00
#define LCD_BLINKON           0x01
#define LCD_BLINKOFF          0x00

        // LCD Cursor Shift
        #define LCD_DISPLAYMOVE       0x08
#define LCD_CURSORMOVE        0x00
#define LCD_MOVERIGHT         0x04
#define LCD_MOVELEFT          0x00

        // LCD Function Set
        #define LCD_8BITMODE          0x10
#define LCD_4BITMODE          0x00
#define LCD_2LINE             0x08
#define LCD_1LINE             0x00
#define LCD_5x10DOTS          0x04
#define LCD_5x8DOTS           0x00

        // Function prototypes
void LCD_Init(unsigned char lcd_addr);
void LCD_Clear(void);
void LCD_Home(void);
void LCD_Display_ON(void);
void LCD_Display_OFF(void);
void LCD_Cursor_ON(void);
void LCD_Cursor_OFF(void);
void LCD_Blink_ON(void);
void LCD_Blink_OFF(void);
void LCD_ScrollDisplayLeft(void);
void LCD_ScrollDisplayRight(void);
void LCD_LeftToRight(void);
void LCD_RightToLeft(void);
void LCD_AutoScroll_ON(void);
void LCD_AutoScroll_OFF(void);
void LCD_CreateCustomChar(unsigned char location, unsigned char* charmap);
void LCD_SetCursor(unsigned char col, unsigned char row);
void LCD_Print_Char(char c);
void LCD_Print_String(const char* str);
void LCD_Command(unsigned char cmd);
#endif