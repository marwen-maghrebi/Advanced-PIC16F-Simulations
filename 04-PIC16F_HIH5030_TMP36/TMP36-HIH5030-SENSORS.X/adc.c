
// adc.c

#include <xc.h>
#include "adc.h"

// Function to configure the ADC module
void ADC_Configure(void) {
    ADCON0 = 0x41;  // Turn ADC ON, select AN0 channel, ADC clock = Fosc/8
    ADCON1 = 0x80;  // All 8 channels are analog, result is "right-justified"
    }

// Function to start ADC conversion
void ADC_StartConversion(void) {
    ADCON0bits.GO_DONE = 1; // Start A/D conversion
    }

// Function to wait for ADC conversion to complete
void ADC_WaitForConversion(void) {
    while (ADCON0bits.GO_DONE); // Polling GO_DONE = delay until conversion is complete
    }

// Function to read ADC result and return the 10-bit value
uint16_t ADC_ReadResult(void) {
    return ((uint16_t)ADRESH << 8) + ADRESL; // Right-justified 10-bit result
}

