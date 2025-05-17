
#include "main.h"
#include "adc.h"
#include "lcd.h"
#include <math.h> // For abs()

// Message buffers
char message1[16];  // For AD592
char message2[16];  // For LM335

// Global variables for temperature
float Celsius_AD592; // Temperature in Celsius for AD592
float Celsius_LM335; // Temperature in Celsius for LM335

// Function Prototypes
void read_LM335_temperature(void);
void read_AD592_temperature(void);
void display_temperatures(void);

// Main function
void main(void) {
    // Initialize peripherals
    lcd_initialize();
    configure_ADC();
    
    while (1) {
        __delay_ms(2000); // Delay for stabilization
        
        // Read and process data from sensors
        read_LM335_temperature();  // Read temperature from LM335
        read_AD592_temperature();  // Read temperature from AD592
        // Display results on the LCD
        display_temperatures();
        }
    }

// Function to read temperature from LM335 (connected to AN0)
void read_LM335_temperature(void) {
    select_ADC_channel(0); // Select AN0 for LM335
    start_ADC_conversion();
    wait_for_conversion();
    float Kelvin = read_ADC_result() * 0.489; // Read analog voltage and convert it to Kelvin (0.489 = 500/1023)
    Celsius_LM335 = Kelvin - 273;            // Convert Kelvin to degree Celsius
    }


void read_AD592_temperature(void) {
    select_ADC_channel(1); // Select AN1 for AD592
    start_ADC_conversion();
    wait_for_conversion();
    float voltage = read_ADC_result() * (5.0 / 1023.0); // Convert ADC result to voltage
    float Kelvin = voltage / 0.01;                      // Convert voltage to Kelvin
    Celsius_AD592 = Kelvin - 273.15;                    // Convert Kelvin to Celsius
    
    }




// Function to display temperatures on the LCD
void display_temperatures(void) {
    // Handle negative temperature for AD592
    if (Celsius_AD592 < 0) {
        sprintf(message1, "AD592: -%2.1f C", fabs(Celsius_AD592)); // Add '-' sign for negative values
        } else {
        sprintf(message1, "AD592: %2.1f C", Celsius_AD592);
        }
    
    // Display AD592 temperature
    lcd_command(0x80); // Move to first line
    lcd_string((const unsigned char *)message1, sizeof(message1));
    
    // Handle negative temperature for LM335
    if (Celsius_LM335 < 0) {
        sprintf(message2, "LM335: -%2.1f C", fabs(Celsius_LM335)); // Add '-' sign for negative values
        } else {
        sprintf(message2, "LM335: %2.1f C", Celsius_LM335);
        }
    
    // Display LM335 temperature
    lcd_command(0xC0); // Move to second line
    lcd_string((const unsigned char *)message2, sizeof(message2));
    }



