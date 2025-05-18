#include "main.h"
#include "adc.h"
#include "lcd.h"

// Servo Control Definitions
#define MIN_PULSE_WIDTH       109   // Minimum pulse width (in timer ticks)
#define MAX_PULSE_WIDTH       221   // Maximum pulse width (in timer ticks)
#define DEFAULT_PULSE_WIDTH   165   // Default pulse width (in timer ticks)
#define TIMER1_RELOAD_VALUE   5000  // Reload value for 20 ms frame (4 MHz clock, 1:8 prescaler)

// Servo Structure
typedef struct {
    uint8_t pin;           // Pin number
    uint16_t pulseWidth;   // Pulse width in timer ticks
} Servo;

// Global Variables
static Servo servo;

// Function Prototypes
void initTimer1(void);
void setupServo(uint8_t pin, uint16_t defaultPulseWidth);
void setServoPulseWidth(uint16_t pulseWidth);
void updateServoPulse(void);
void displayReadings(float voltage, float angle);
void calculatePulseWidthAndAngle(uint16_t adcResult, uint16_t* pulseWidth, float* angle);

// Timer1 Interrupt Service Routine
void __interrupt() ISR(void) {
    if (TMR1IF) {
        TMR1IF = 0;  // Clear Timer1 interrupt flag
        updateServoPulse();

        // Reload Timer1 for the next 20 ms frame
        TMR1H = TIMER1_RELOAD_VALUE >> 8;
        TMR1L = TIMER1_RELOAD_VALUE & 0xFF;
    }
}

// Main Function
void main(void) {
    uint16_t adcResult = 0;
    float voltage = 0.0;
    uint16_t pulseWidth = DEFAULT_PULSE_WIDTH;
    float angle = 0.0;

    // Initialize peripherals
    lcd_initialize();
    configure_ADC();
    setupServo(0, DEFAULT_PULSE_WIDTH);  // Initialize servo on RE0 with default pulse width

    while (1) {
        // Read ADC and calculate values
        start_ADC_conversion();
        wait_for_conversion();
        adcResult = read_ADC_result();

        // Calculate pulse width and angle
        calculatePulseWidthAndAngle(adcResult, &pulseWidth, &angle);

        // Update servo and display
        setServoPulseWidth(pulseWidth);
        displayReadings((adcResult * 5.0) / 1023.0, angle);

        __delay_ms(500);
    }
}

// Initialize Timer1 for Servo Control
void initTimer1(void) {
    TMR1H = TIMER1_RELOAD_VALUE >> 8;  // Set Timer1 high byte
    TMR1L = TIMER1_RELOAD_VALUE & 0xFF; // Set Timer1 low byte

    T1CON = 0x31;  // Timer1 ON, Prescaler = 1:8
    TMR1IF = 0;    // Clear Timer1 interrupt flag
    TMR1IE = 1;    // Enable Timer1 interrupt
    PEIE = 1;      // Enable peripheral interrupts
    GIE = 1;       // Enable global interrupts
}

// Setup Servo on a Specific Pin with a Default Pulse Width
void setupServo(uint8_t pin, uint16_t defaultPulseWidth) {
    servo.pin = pin;  // Assign pin
    servo.pulseWidth = defaultPulseWidth;  // Set default pulse width
    TRISE &= ~(1 << servo.pin);  // Set pin as output
    PORTE &= ~(1 << servo.pin);  // Initialize pin to LOW
    initTimer1();  // Initialize Timer1
}

// Update the Servo Pulse (called in ISR)
void updateServoPulse(void) {
    PORTE |= (1 << servo.pin);  // Set pin HIGH
    for (uint16_t i = 0; i < servo.pulseWidth; i++) __asm("nop");  // Wait for pulse width
    PORTE &= ~(1 << servo.pin);  // Set pin LOW
}

// Set Servo Pulse Width
void setServoPulseWidth(uint16_t pulseWidth) {
    if (pulseWidth < MIN_PULSE_WIDTH) pulseWidth = MIN_PULSE_WIDTH;
    if (pulseWidth > MAX_PULSE_WIDTH) pulseWidth = MAX_PULSE_WIDTH;
    servo.pulseWidth = pulseWidth;
}

// Calculate Pulse Width and Angle from ADC Result
void calculatePulseWidthAndAngle(uint16_t adcResult, uint16_t* pulseWidth, float* angle) {
    float voltage = (adcResult * 5.0) / 1023.0;  // Convert ADC result to voltage
    *pulseWidth = (uint16_t)((voltage * (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH)) / 5.0) + MIN_PULSE_WIDTH;
    *angle = (*pulseWidth - MIN_PULSE_WIDTH) * 180.0 / (MAX_PULSE_WIDTH - MIN_PULSE_WIDTH) - 90.0;
}

// Display Voltage and Angle on LCD
void displayReadings(float voltage, float angle) {
    char displayBuffer[16];

    // Display voltage
    lcd_command(0x80);
    sprintf(displayBuffer, "Volt: %.2fV", voltage);
    lcd_string(displayBuffer, 16);

    // Display angle
    lcd_command(0xC0);
    sprintf(displayBuffer, "Angle: %.1f", angle);
    lcd_string(displayBuffer, 16);
}
