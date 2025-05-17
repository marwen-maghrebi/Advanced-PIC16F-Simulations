/**
 * @file main.c
 * @brief LCD Display Demo Application
 * @details Demonstrates various LCD functions and custom characters
 */

#include <xc.h>
#include <stdio.h>
#include "i2c.h"
#include "jhd_lcd.h"

/* Constants */
#define LCD_ADDRESS     0x7C
#define DELAY_SHORT     500
#define DELAY_MEDIUM    1000
#define DELAY_LONG      2000
#define DELAY_EXTRA     3000
#define SCROLL_COUNT    5
#define AUTOSCROLL_COUNT 10

/* Custom Character Definitions */
const unsigned char SMILEY[8] = {
    0b00000, 0b01010, 0b00000, 0b00000,
    0b10001, 0b01110, 0b00000, 0b00000
};

const unsigned char LAUGHING_SMILEY[8] = {
    0b00000, 0b01010, 0b00000, 0b00000,
    0b10001, 0b11111, 0b01110, 0b00000
};

/* Function Prototypes */
void systemInit(void);
void demoBasicPrinting(void);
void demoAutoScroll(void);
void demoCursorControl(void);
void demoDisplayControl(void);
void demoTextDirection(void);
void demoScrolling(void);
void demoHomeCommand(void);

/**
 * @brief Main program entry point
 */
void main(void) {
    systemInit();
    
    while (1) {
        demoBasicPrinting();
        demoAutoScroll();
        demoCursorControl();
        demoDisplayControl();
        demoTextDirection();
        demoScrolling();
        demoHomeCommand();
    }
}

/**
 * @brief Initialize system components and custom characters
 */
void systemInit(void) {
    I2C_Master_Init();
    LCD_Init(LCD_ADDRESS);
    LCD_Clear();
    
    // Initialize custom characters
    LCD_CreateCustomChar(0, SMILEY);
    LCD_CreateCustomChar(1, LAUGHING_SMILEY);
}

/**
 * @brief Demonstrate basic text printing and custom characters
 */
void demoBasicPrinting(void) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Hello! Like");
    LCD_SetCursor(0, 1);
    LCD_Print_String("SUBSCRIBE ");
    
    // Display custom characters
    LCD_Print_Char(0);
    LCD_Print_Char(1);
    LCD_Print_Char(0);
    
    __delay_ms(DELAY_EXTRA);
}

/**
 * @brief Demonstrate auto-scroll functionality
 */
void demoAutoScroll(void) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("AutoScroll Test");
    LCD_AutoScroll_ON();
    
    for (int i = 0; i < AUTOSCROLL_COUNT; i++) {
        LCD_Print_Char('.');
        __delay_ms(DELAY_SHORT);
    }
    
    LCD_AutoScroll_OFF();
    __delay_ms(DELAY_LONG);
}

/**
 * @brief Demonstrate cursor and blink control
 */
void demoCursorControl(void) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Cursor ON");
    LCD_Cursor_ON();
    __delay_ms(DELAY_MEDIUM);
    
    LCD_SetCursor(0, 1);
    LCD_Print_String("Blink ON");
    LCD_Blink_ON();
    __delay_ms(DELAY_MEDIUM);
    
    LCD_Blink_OFF();
    LCD_Cursor_OFF();
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Cursor/Blink OFF");
    __delay_ms(DELAY_LONG);
}

/**
 * @brief Demonstrate display on/off control
 */
void demoDisplayControl(void) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Display OFF");
    LCD_Display_OFF();
    __delay_ms(DELAY_MEDIUM);
    
    LCD_Display_ON();
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Display ON");
    __delay_ms(DELAY_LONG);
}

/**
 * @brief Demonstrate text direction control
 */
void demoTextDirection(void) {
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Text L->R");
    LCD_LeftToRight();
    __delay_ms(DELAY_LONG);
    
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Text R->L");
    LCD_RightToLeft();
    __delay_ms(DELAY_LONG);
}

/**
 * @brief Demonstrate display scrolling
 */
void demoScrolling(void) {
    // Left scrolling demo
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Scroll Left");
    for (int i = 0; i < SCROLL_COUNT; i++) {
        __delay_ms(DELAY_SHORT);
        LCD_ScrollDisplayLeft();
    }
    
    // Right scrolling demo
    LCD_Clear();
    LCD_SetCursor(0, 0);
    LCD_Print_String("Scroll Right");
    for (int i = 0; i < SCROLL_COUNT; i++) {
        __delay_ms(DELAY_SHORT);
        LCD_ScrollDisplayRight();
    }
}

/**
 * @brief Demonstrate home command
 */
void demoHomeCommand(void) {
    LCD_Clear();
    LCD_SetCursor(0, 1);
    LCD_Print_String("Returning Home");
    __delay_ms(DELAY_LONG);
    
    LCD_Home();
    LCD_Print_String("Home");
    __delay_ms(DELAY_LONG);
}