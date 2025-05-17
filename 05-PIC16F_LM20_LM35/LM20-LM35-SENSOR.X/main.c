#include <xc.h>
#include <stdio.h>
#include <math.h>
#include "uart.h"

// Configuration bits
#pragma config FOSC = HS        // High Speed Crystal
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low Voltage Programming disabled
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection disabled
#pragma config WRT = OFF        // Flash Program Memory Write disabled
#pragma config CP = OFF         // Flash Program Memory Code Protection disabled

#define _XTAL_FREQ 8000000     // 8MHz Crystal
#define VREF 5.0f              // Reference voltage

// Function prototypes
void Initialize_ADC(void);
unsigned int Read_ADC(unsigned char channel);
float Convert_To_Temperature_LM20(unsigned int adc_value);
float Convert_To_Temperature_LM35(unsigned int adc_value);
void Display_Temperature(const char* sensor_name, float temperature);

// Initialize ADC for multiple channels

void Initialize_ADC(void) {
    TRISA = 0x03;     // Set RA0 and RA1 as input (binary: 0000 0011)
    ADCON1 = 0x8E;    // Configure AN0 and AN1 as analog, others digital
    ADCON0 = 0x41;    // ADC ON, Channel 0, Fosc/16
    
    __delay_ms(10);   // Wait for ADC to stabilize
}


// Read ADC value from specified channel
unsigned int Read_ADC(unsigned char channel) {
    ADCON0bits.CHS = channel;    // Select ADC channel
    __delay_ms(2);               // Wait for channel to set up
    ADCON0bits.GO = 1;           // Start conversion
    while(ADCON0bits.GO_nDONE);  // Wait for conversion to complete
    return (ADRESH << 8) + ADRESL;
}

// Convert ADC value to temperature for LM20
float Convert_To_Temperature_LM20(unsigned int adc_value) {
    float Vo = ((float)adc_value * VREF) / 1023.0f;
    float temp = -1481.96f + sqrtf(2.1962e6f + ((1.8639f - Vo) / (3.88e-6f)));
    return temp;
}

// Convert ADC value to temperature for LM35
float Convert_To_Temperature_LM35(unsigned int adc_value) {
    float Vo = ((float)adc_value * VREF) / 1023.0f;
    float temp = Vo * 100.0f;  // LM35 outputs 10mV per °C, so multiply by 100
    return temp;
}

// Display temperature function implementation
void Display_Temperature(const char* sensor_name, float temperature) {
    char buffer[50];
    sprintf(buffer, "%s Temperature: %.2f°C\n\r", sensor_name, temperature);
    UART_Write_Text(buffer);
}

// Main function
void main(void) {
    unsigned int adc_value_LM20, adc_value_LM35;
    float temperature_LM20, temperature_LM35;
    
    Initialize_ADC();
    UART_TX_Init();
    
    while(1) {
        // Read temperature from LM20 (connected to AN0)
        adc_value_LM20 = Read_ADC(0);  // Channel 0 for LM20
        temperature_LM20 = Convert_To_Temperature_LM20(adc_value_LM20);
        Display_Temperature("LM20", temperature_LM20);
        
        // Read temperature from LM35 (connected to AN1)
        adc_value_LM35 = Read_ADC(1);  // Channel 1 for LM35
        temperature_LM35 = Convert_To_Temperature_LM35(adc_value_LM35);
        Display_Temperature("LM35", temperature_LM35);
        UART_Write_Text("----------------------------\n\r");

        __delay_ms(1000);  // Wait before next reading
    }
}
