/*
 * File: main.c
 * Author: Marwen Maghrebi
 * Description: This code demonstrates how to interface an LCD with a PIC microcontroller
 *              to display messages using professional function names and structured comments.
 */
 
#include <xc.h>
 
#define _XTAL_FREQ  10000000UL      // 10MHz
 
// Configuration bits
#pragma config FOSC = HS            // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF           // Watchdog Timer Enable bit (WDT Disabled)
#pragma config PWRTE = OFF          // Power-up Timer Enable bit (PWRT disabled)
#pragma config CP = OFF             // FLASH Program Memory Code Protection bits (Code protection off)
#pragma config BOREN = ON           // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF            // Low Voltage In-Circuit Serial Programming Disable bit (RB3/PGM pin has PGM function; low-voltage programming enabled)
#pragma config CPD = OFF            // Data EE Memory Code Protection (Code Protection off)
#pragma config WRT = OFF            // FLASH Program Memory Write Disable (Unprotected program memory may be written to by EECON control)
 
#define LCD_BUS             PORTB   // Data bus for LCD (8 bits)
#define ENABLE_LCD          PORTCbits.RC1 // Enable pin of LCD
#define RS_LCD              PORTCbits.RC0 // RS pin of LCD
 
#define LCD_BUS_DIRECTION   TRISB   // Data bus tristate register
#define ENABLE_DIRECTION    TRISCbits.TRISC1    // Enable pin tristate register
#define RS_DIRECTION        TRISCbits.TRISC0    // RS pin tristate register
 
#define Line0               0x80
#define Line1               0xC0
 
// Function prototypes
void initializeLCD(void);
void clearLCD(void);
void sendCommandToLCD(unsigned char command);
void sendDataToLCD(unsigned char data);
void printToLCD(const char *string);
unsigned char moveCursorToPosition(unsigned char address);
 
// Function implementations
 
void initializeLCD(void) {
    // Set pins as output
    ENABLE_DIRECTION    = 0;
    RS_DIRECTION        = 0;
    LCD_BUS_DIRECTION   = 0;
 
    // Write zero to pins and port
    ENABLE_LCD  = 0;
    RS_LCD      = 0;
    LCD_BUS     = 0;
 
    __delay_ms(10);          // 10 milliseconds delay
 
    // Initialization commands for LCD
    sendCommandToLCD(0x38); // Function set as given in datasheet
    sendCommandToLCD(0x0F); // Display ON; Cursor ON; Blink ON
    sendCommandToLCD(0x06); // Display shifting OFF
    clearLCD();             // Clear screen command
}
 
void clearLCD(void) {
    sendCommandToLCD(0x01); // Clear screen command
    __delay_ms(3);          // Delay for cursor to return home (minimum 3ms)
}
 
void sendCommandToLCD(unsigned char command) {
    RS_LCD = 0;             // Command RS must be low (0)
    LCD_BUS = command;      // Write command to data bus of LCD
    ENABLE_LCD = 1;         // Toggle Enable PIN to display data
    __delay_us(200);
    ENABLE_LCD = 0;
    __delay_us(200);
}
 
void sendDataToLCD(unsigned char data) {
    RS_LCD = 1;             // Data RS must be high (1)
    LCD_BUS = data;         // Write data to data bus of LCD
    ENABLE_LCD = 1;         // Toggle Enable PIN to display data
    __delay_us(200);
    ENABLE_LCD = 0;
    __delay_us(200);
}
 
void printToLCD(const char *string) {
    while(*string) {
        sendDataToLCD(*string++);   // Display data until string ends
    }
}
 
unsigned char moveCursorToPosition(unsigned char address) {
    // Valid addresses: 0x80 to 0xA8 for line one, 0xC0 to 0xE8 for line two
    if ((address >= 0x80 && address <= 0xA8) || (address >= 0xC0 && address <= 0xE8)) {
        sendCommandToLCD(address);
        return 1;
    } else {
        return 0;
    }
}
 
int main() {
    // Set pins as output
    ENABLE_DIRECTION    = 0;
    RS_DIRECTION        = 0;
    LCD_BUS_DIRECTION   = 0;
 
    // Write zero to pins and port
    ENABLE_LCD  = 0;
    RS_LCD      = 0;
    LCD_BUS     = 0;
 
    __delay_ms(10);          // 10 milliseconds delay
 
    // Initialization commands for LCD
    sendCommandToLCD(0x38); // Function set as given in datasheet
    sendCommandToLCD(0x0F); // Display ON; Cursor ON; Blink ON
    sendCommandToLCD(0x06); // Display shifting OFF
    sendCommandToLCD(0x01); // Clear screen command
 
    while(1) {
       
 
        moveCursorToPosition(Line0);
        printToLCD("  THE EMBEDDED  ");
 
        moveCursorToPosition(Line1);
        printToLCD("     THINGS     ");
        __delay_ms(2000);           // 2 Second Delay.

        clearLCD();

        moveCursorToPosition(Line0);
        printToLCD("**  SUBSCIBE **");
 
        moveCursorToPosition(Line1);
        printToLCD("**   LIKE   **");
        __delay_ms(2000);           // 2 Second Delay.
          
       // __delay_ms(100);
    }
    return 0;           
}