
#include "MCP980x.h"
#include "lcd.h"
#include <xc.h>
#include <stdio.h>

char displayString[16];

void initializeHardware(void);
void configureOneShotMode(void);
void configureShutdownMode(void);
void configureComparatorMode(void);
void configureInterruptMode(void);
void handleButtonPress(void);

int main(void) {
    
    initializeHardware();
    clearLCD();
    moveCursorToPosition(0);
    printToLCD("MCP9800 Ready");
    __delay_ms(1000);
    
    while (1) {
        handleButtonPress();
        float temp = MCP9800_ReadTemperature();
        moveCursorToPosition(Line1);
        snprintf(displayString, sizeof(displayString), "Temp: %.2f C", temp);
        moveCursorToPosition(0);
        printToLCD(displayString);
        __delay_ms(2000);
        }
    return 0;
    }

void initializeHardware(void) {
    TRISAbits.TRISA0 = 1;
    TRISAbits.TRISA1 = 1;
    TRISAbits.TRISA2 = 1;
    
    ADCON1 = 0x06;
    
    initializeLCD();
    MCP9800_Init();
    }

void handleButtonPress(void) {
    if (RA0 == 1) {
        configureOneShotMode();
        clearLCD();
        moveCursorToPosition(Line0);
        printToLCD("Mode: One-Shot");
    } else if (RA1 == 1) {
        configureComparatorMode();
        clearLCD();
        moveCursorToPosition(Line0);
        printToLCD("Mode: Comparator");
    } else if (RA2 == 1) {
        configureInterruptMode();
        clearLCD();
        moveCursorToPosition(Line0);
        printToLCD("Mode: Interrupt");
    }
}

void configureOneShotMode(void) {
    MCP9800_SetConfigResolution(ADC_RES_9BIT);
    MCP9800_SetConfigShutdown(1);
    MCP9800_SetConfigOneShot(1);
    __delay_ms(30);
    }

void configureContinuousConversionMode(void){
    MCP9800_SetConfigResolution(ADC_RES_9BIT);
    MCP9800_SetConfigShutdown(0);
    MCP9800_SetConfigOneShot(0);
    __delay_ms(30);
    }

void configureComparatorMode(void) {
    configureContinuousConversionMode();
    MCP9800_SetConfigShutdown(1);
    MCP9800_SetLIMIT_Temp(60);
    MCP9800_SetHYST_Temp(55);
    MCP9800_SetConfigShutdown(0);
    MCP9800_SetConfigAlertPolarity(0);
    MCP9800_SetConfigResolution(ADC_RES_12BIT);
    MCP9800_SetConfigFaultQueue(MCP_4_FAULTS);
    MCP9800_SetConfigCompIntMode(MCP_COMPARATOR);
    }

void configureInterruptMode(void) {
    configureContinuousConversionMode();
    MCP9800_SetConfigShutdown(1);
    MCP9800_SetLIMIT_Temp(70);
    MCP9800_SetHYST_Temp(65);
    MCP9800_SetConfigShutdown(0);
    MCP9800_SetConfigAlertPolarity(0);
    MCP9800_SetConfigResolution(ADC_RES_12BIT);
    MCP9800_SetConfigFaultQueue(MCP_2_FAULTS);
    MCP9800_SetConfigCompIntMode(MCP_INTERRUPT);
    }
