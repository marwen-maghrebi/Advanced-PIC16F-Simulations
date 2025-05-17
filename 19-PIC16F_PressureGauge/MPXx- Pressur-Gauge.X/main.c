
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

#define _XTAL_FREQ 16000000     // 16MHz Crystal

// Define ADC Channels
#define PRESS_A6115 0           // Analog channel for MPXA6115A6U
#define SENSOR_MPX4250 1        // Analog channel for MPX4250
#define SENSOR_MPX4115A 2       // Analog channel for MPX4115A

// Define Constants for Pressure Calculations
#define SENSITIVITY_A6115 0.04585  // Sensitivity in V/kPa
#define OFFSET_A6115 -0.108        // Offset in V
#define ERROR_A6115 10             // Calibration error in kPa
#define VREF_A6115 5.0             // Reference Voltage for ADC

#define SENSITIVITY_MPX4250 0.0188 // Sensitivity in V/kPa
#define OFFSET_MPX4250 0.04        // Offset in V
#define ERROR_MPX4250 -10             // Calibration error in kPa
#define VREF_MPX4250 5.0             // Reference Voltage for ADC

#define SENSITIVITY_MPX4115A 0.0459 // Sensitivity in V/kPa
#define OFFSET_MPX4115A -0.095      // Offset in V
#define ERROR_MPX4115A  5.5           // Calibration error in kPa
#define VREF_MPX4115A   5.1         // Reference Voltage for ADC

#define ADC_RESOLUTION 1023.0       // ADC Resolution
volatile int peakMemory_MPX4250 = 0; // Peak pressure memory for MPX4250

// Function Prototypes
void Initialize_ADC(void);
unsigned int Read_ADC(unsigned char channel);
float pressure_A6115(void);
float readPressure_MPX4250(void);
float readPressure_MPX4115A(void);


// Initialize ADC
void Initialize_ADC(void) {
    TRISA = 0x07;     // Set RA0, RA1, and RA2 as inputs
    ADCON1 = 0x80;    // Configure AN0, AN1, and AN2 as analog, others digital
    ADCON0 = 0x41;    // ADC ON, Fosc/16
    __delay_ms(10);   // Stabilize ADC
    }

// Read ADC Value
unsigned int Read_ADC(unsigned char channel) {
    ADCON0bits.CHS = channel;  // Select ADC channel
    __delay_ms(2);             // Delay for channel stabilization
    ADCON0bits.GO = 1;         // Start ADC conversion
    while (ADCON0bits.GO_nDONE); // Wait for conversion to complete
    return (ADRESH << 8) + ADRESL; // Return 10-bit ADC result
    }

// Calculate pressure for MPXA6115A6U
float pressure_A6115() {
    unsigned int adcValue = Read_ADC(PRESS_A6115);
    float Vout = (adcValue / ADC_RESOLUTION) * VREF_A6115;  // Convert ADC to voltage
    return ((Vout - OFFSET_A6115) / SENSITIVITY_A6115) + ERROR_A6115; // Pressure in kPa
    }

// Calculate pressure for MPX4250
float readPressure_MPX4250() {
    unsigned int adcValue = Read_ADC(SENSOR_MPX4250);
    float Vout = (adcValue / ADC_RESOLUTION) * VREF_MPX4250; // Convert ADC to voltage
    float pressure = (Vout - OFFSET_MPX4250) / SENSITIVITY_MPX4250; // Pressure in kPa
    
    // Update peak memory
    if (pressure > peakMemory_MPX4250) {
        peakMemory_MPX4250 = pressure+ERROR_MPX4250;
        }
    return pressure + ERROR_MPX4250;
    }

// Calculate pressure for MPX4115A
float readPressure_MPX4115A() {
    unsigned int adcValue = Read_ADC(SENSOR_MPX4115A);
    float Vout = (adcValue / ADC_RESOLUTION) * VREF_MPX4115A;  // Convert ADC to voltage
    return ((Vout - OFFSET_MPX4115A) / SENSITIVITY_MPX4115A)+ERROR_MPX4115A; // Pressure in kPa
    }

// Main Function
void main(void) {
    Initialize_ADC();
    UART_TX_Init();
    UART_Write_Text("Pressure Sensor System Initialized.\n\r");
    char buffer[50];
    
    while (1) {
        // Read and display MPXA6115A6U pressure
        float pressure_A6115_val = pressure_A6115();
        sprintf(buffer, "1-[MPXA6115A6U] Pressure: %.2f kPa\n\r", pressure_A6115_val);
        UART_Write_Text(buffer);
        
        // Read and display MPX4250 pressure
        float pressure_MPX4250_val = readPressure_MPX4250();
        sprintf(buffer, "2-[MPX4250] Current Pressure: %.2f kPa\n\r",pressure_MPX4250_val);
        UART_Write_Text(buffer);
        
        // Read and display MPX4115A pressure
        float pressure_MPX4115A_val = readPressure_MPX4115A();
        sprintf(buffer, "3-[MPX4115A] Pressure: %.2f kPa\n\r", pressure_MPX4115A_val);
        UART_Write_Text(buffer);
        UART_Write_Text("-----------------------------------------------\n\r");
        
        __delay_ms(2000); // Delay before the next reading
        }
    }


