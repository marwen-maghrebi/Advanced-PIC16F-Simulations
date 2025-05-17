
#include "main.h"
#include "lcd.h"
#include <stdio.h>
#include <string.h>

// Constants for ADC calculations
#define V_REF 5.0           // ADC reference voltage
#define ADC_RESOLUTION 1024 // 10-bit ADC resolution (0 to 1023)
#define NUM_READINGS 100    // Number of readings for averaging
#define VSUPPLY 3.3

// Global variables
float Vout;
uint16_t ADC_VALUE;
int correctionFactorRH = 0; // Humidity correction factor (if needed)

// Function prototypes
void configure_ADC(void);
void start_ADC_conversion(void);
void wait_for_conversion(void);
uint16_t read_ADC_result(void);
double readHumidity(void);
double readTemperature(void);

void main(void) {
    // Configure LCD control and data pins as output
    TRISC &= ~(1 << 0 | 1 << 1 | 1 << 2);
    TRISD &= ~(1 << 4 | 1 << 5 | 1 << 6 | 1 << 7);
    
    lcd_initialize(); // Initialize the LCD
    configure_ADC();  // Configure ADC module
    
    while (1) {
        // Get the temperature reading from TMP36
        double temperature_value = readTemperature();
        
        // Format temperature value for display
        char temperature_str[16];
        sprintf(temperature_str,"Temp : %.2f C", temperature_value);
        
        // Get the humidity reading (averaged from multiple readings)
        double humidity_value = readHumidity();
        
        // Format humidity value for display
        char humidity_str[16];
        sprintf(humidity_str, "Hum  : %.2f %%", humidity_value);
        
        // Display temperature on the 1st row
        lcd_command(0x80); // Position cursor at 1st Row, 1st Column
        lcd_string((const unsigned char *)temperature_str, strlen(temperature_str));
        
        // Display humidity on the 2nd row
        lcd_command(0xC0); // Position cursor at 2nd Row, 1st Column
        lcd_string((const unsigned char *)humidity_str, strlen(humidity_str));
    }
}

// Function to configure the ADC module
void configure_ADC(void) {
    ADCON0 = 0x01; // Turn ADC ON, select AN0 channel, ADC clock = Fosc/8
    ADCON1 = 0x80; // All 8 channels are analog, result is "right-justified"
}

// Function to start ADC conversion
void start_ADC_conversion(void) {
ADCON0bits.GO_DONE = 1; // Start A/D conversion
}

// Function to wait for ADC conversion to complete
void wait_for_conversion(void) {
    while (ADCON0bits.GO_DONE); // Polling GO_DONE until conversion is complete
    }

// Function to read ADC result
uint16_t read_ADC_result(void) {
    uint16_t ADC_result = ((uint16_t)ADRESH << 8) + ADRESL; // Read the right-justified 10-bit result
    return ADC_result;
}

// Function to read humidity sensor value from AN0
double readHumidity(void) {
    double sum = 0.0;
    
    // Configure ADC to use channel AN0
    ADCON0 &= 0xC7; // Clear previous channel selection bits
    ADCON0 |= (0 << 3); // Select AN0 channel
    
    // Take multiple readings and average them
    for (int i = 0; i < NUM_READINGS; i++) {
        start_ADC_conversion();  // Start ADC conversion
        wait_for_conversion();   // Wait for conversion to complete
        ADC_VALUE = read_ADC_result(); // Read ADC result
        Vout = ADC_VALUE * V_REF / ADC_RESOLUTION;
        
        // Calculate the humidity reading
        double humidity_value = (((Vout / VSUPPLY) - 0.1515) / 0.00636) + correctionFactorRH;
        sum += humidity_value;
    }
   
// Return the average humidity value
return sum / NUM_READINGS;
}

// Function to read temperature sensor (TMP36) value from AN1
double readTemperature(void) {
    double sum = 0.0;
    
    // Configure ADC to use channel AN1
    ADCON0 &= 0xC7; // Clear previous channel selection bits
    ADCON0 |= (1 << 3); // Select AN1 channel
    
    // Take multiple readings and average them
    for (int i = 0; i < NUM_READINGS; i++) {
        start_ADC_conversion();  // Start ADC conversion
        wait_for_conversion();   // Wait for conversion to complete
        ADC_VALUE = read_ADC_result(); // Read ADC result
        Vout = ADC_VALUE * V_REF / ADC_RESOLUTION;
        
        // Calculate temperature in Celsius for TMP36
        double temperature_value = (Vout - 0.5) * 100.0;
        
        sum += temperature_value;
        }
    
    // Return the average temperature value
    return sum / NUM_READINGS;
    }

