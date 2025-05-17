
#include "adc.h"

// Function to configure the ADC module
void configure_ADC(void) {
    TRISA = 0x03;     // Set RA0 and RA1 as input (binary: 0000 0011)
    ADCON1 = 0x80;      // Configure AN0 and AN1 as analog, others digital
    ADCON0 = 0x41;    // ADC ON, Channel 0, Fosc/16
    __delay_ms(10);   // Wait for ADC to stabilize
    }

// Function to select ADC channel
void select_ADC_channel(unsigned char channel) {
    ADCON0bits.CHS = channel;    // Select ADC channel
    __delay_ms(2);                // Wait for channel to set up
    }

// Function to start ADC conversion
void start_ADC_conversion(void) {
    __delay_us(10);            // Allow acquisition time
    ADCON0bits.GO_DONE = 1;    // Start ADC conversion
    }

// Function to wait for ADC conversion to complete
void wait_for_conversion(void) {
    while (ADCON0bits.GO_DONE); // Polling GO_DONE = delay until conversion is complete
    }

// Read ADC result
uint16_t read_ADC_result(void) {
    return ((uint16_t)ADRESH << 8) | ADRESL;  // Combine high and low result
    }
