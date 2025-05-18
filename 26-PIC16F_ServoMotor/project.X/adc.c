#include "adc.h"

// Function to configure the ADC module
void configure_ADC(void) {
    ADCON0 = 0x41;  // Turn ADC ON, select AN0 channel, ADC clock = Fosc/8
    ADCON1 = 0x80;  // All 8 channels are analog, result is "right-justified"
                    // ADC clock = Fosc/8
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
