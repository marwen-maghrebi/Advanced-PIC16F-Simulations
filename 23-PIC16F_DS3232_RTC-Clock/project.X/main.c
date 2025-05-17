/**
 * @file main.c
 * @brief LCD Display with DS3231 RTC
 * @details Real-time clock display with setting capabilities for PIC16F877A
 */
#include <xc.h>
#include <stdio.h>
#include "i2c.h"
#include "jhd_lcd.h"

// PIC16F877A Configuration Bit Settings
#pragma config FOSC = HS   // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF  // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = ON  // Brown-out Reset Enable bit (BOR enabled)
#pragma config LVP = OFF   // Low-Voltage Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF   // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF   // Flash Program Memory Write Enable bits (Write protection off)
#pragma config CP = OFF    // Flash Program Memory Code Protection bit (Code protection off)

// Constants
#define LCD_ADDRESS     0x7C
#define DS3231_ADDRESS  0xD0
#define BUTTON1         PORTBbits.RB0
#define BUTTON2         PORTBbits.RB1
#define _XTAL_FREQ     16000000    

// Global variables
char time[]     = "TIME:  :  :  ";
char calendar[] = "DATE:  /  /20  ";
uint8_t i, second, minute, hour, date, month, year;

// Function prototypes
void DS3231_display(void);
void blink(void);
uint8_t edit(uint8_t parameter, uint8_t xx, uint8_t yy);

// Convert BCD to decimal
uint8_t bcd_to_decimal(uint8_t bcd) {
    return ((bcd >> 4) * 10 + (bcd & 0x0F));
}

// Convert decimal to BCD
uint8_t decimal_to_bcd(uint8_t decimal) {
    uint8_t tens = decimal / 10;
    uint8_t ones = decimal % 10;
    return ((tens << 4) | ones);
}

void DS3231_display(void) {
    // Convert BCD to decimal
    second = bcd_to_decimal(second);
    minute = bcd_to_decimal(minute);
    hour   = bcd_to_decimal(hour);
    date   = bcd_to_decimal(date);
    month  = bcd_to_decimal(month);
    year   = bcd_to_decimal(year);

    // Update time string
    time[12] = second % 10 + '0';
    time[11] = second / 10 + '0';
    time[9] = minute % 10 + '0';
    time[8] = minute / 10 + '0';
    time[6] = hour % 10 + '0';
    time[5] = hour / 10 + '0';

    // Update calendar string
    calendar[14] = year % 10 + '0';
    calendar[13] = year / 10 + '0';
    calendar[9] = month % 10 + '0';
    calendar[8] = month / 10 + '0';
    calendar[6] = date % 10 + '0';
    calendar[5] = date / 10 + '0';

    // Display on LCD
    LCD_SetCursor(0, 0);
    LCD_Print_String(time);
    LCD_SetCursor(0, 1);
    LCD_Print_String(calendar);
}

void blink(void) {
    uint8_t j = 0;
    while(j < 10 && BUTTON1 && BUTTON2) {
        j++;
        __delay_ms(25);
    }
}

uint8_t edit(uint8_t parameter, uint8_t xx, uint8_t yy) {
    char temp[3];
    
    while(!BUTTON1);  // Wait until button RB0 is released
    
    while(1) {
        while(!BUTTON2) {  // If button RB1 is pressed
            parameter++;
            if(i == 0 && parameter > 23)        // Hours
                parameter = 0;
            if(i == 1 && parameter > 59)        // Minutes
                parameter = 0;
            if(i == 2 && parameter > 31)        // Date
                parameter = 1;
            if(i == 3 && parameter > 12)        // Month
                parameter = 1;
            if(i == 4 && parameter > 99)        // Year
                parameter = 0;
                
            LCD_SetCursor(xx, yy);
            sprintf(temp, "%02u", parameter);
            LCD_Print_String(temp);
            __delay_ms(200);
        }
        
        // Blink effect
        LCD_SetCursor(xx, yy);
        LCD_Print_String("  ");
        blink();
        LCD_SetCursor(xx, yy);
        sprintf(temp, "%02u", parameter);
        LCD_Print_String(temp);
        blink();
        
        if(!BUTTON1) {  // If button RB0 is pressed
            i++;
            return parameter;
        }
    }
}

void DS3231_write(void) {
    I2C_Master_Start();
    I2C_Master_Write(DS3231_ADDRESS);
    I2C_Master_Write(0x00);  // Start at register 0
    I2C_Master_Write(0x00);  // Seconds = 0
    I2C_Master_Write(decimal_to_bcd(minute));
    I2C_Master_Write(decimal_to_bcd(hour));
    I2C_Master_Write(1);     // Day (not used)
    I2C_Master_Write(decimal_to_bcd(date));
    I2C_Master_Write(decimal_to_bcd(month));
    I2C_Master_Write(decimal_to_bcd(year));
    I2C_Master_Stop();
}

void DS3231_read(void) {
    I2C_Master_Start();
    I2C_Master_Write(DS3231_ADDRESS);
    I2C_Master_Write(0x00);  // Start at register 0
    
    I2C_Master_RepeatedStart();
    I2C_Master_Write(DS3231_ADDRESS | 0x01);  // Read mode
    
    second = I2C_Read_Byte(); I2C_ACK();
    minute = I2C_Read_Byte(); I2C_ACK();
    hour = I2C_Read_Byte(); I2C_ACK();
    I2C_Read_Byte(); I2C_ACK();  // Day (not used)
    date = I2C_Read_Byte(); I2C_ACK();
    month = I2C_Read_Byte(); I2C_ACK();
    year = I2C_Read_Byte(); I2C_NACK();
    
    I2C_Master_Stop();
}

void main(void) {
    // Initialize ports
    TRISB = 0xFF;          // Set PORTB as input for buttons
    OPTION_REGbits.nRBPU = 0;  // Enable PORTB pull-ups globally
    
    // Initialize I2C and LCD
    I2C_Master_Init();
    LCD_Init(LCD_ADDRESS);
    LCD_Clear();
    
    while(1) {
        if(!BUTTON1) {  // If RB0 button is pressed
            i = 0;
            hour = edit(hour, 5, 0);
            minute = edit(minute, 8, 0);
            date = edit(date, 5, 1);
            month = edit(month, 8, 1);
            year = edit(year, 13, 1);
            
            DS3231_write();
            __delay_ms(200);
        }
        
        DS3231_read();
        DS3231_display();
        __delay_ms(50);
    }
}