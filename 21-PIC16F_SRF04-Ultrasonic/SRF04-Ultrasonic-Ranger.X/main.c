
#include <xc.h>
#include <stdint.h>
#include <stdio.h>

// Configuration bits
#pragma config FOSC = HS        // High-Speed Crystal
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low Voltage Programming disabled
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection disabled
#pragma config WRT = OFF        // Flash Program Memory Write disabled
#pragma config CP = OFF         // Flash Program Memory Code Protection disabled

#define _XTAL_FREQ 20000000     // 20 MHz oscillator frequency

// Pin Definitions
#define TRIG_PIN RB0            // Trigger pin connected to RB0
#define ECHO_PIN RB1            // Echo pin connected to RB1
#define TRIG_PIN_DIR TRISB0     // Direction register for TRIG_PIN
#define ECHO_PIN_DIR TRISB1     // Direction register for ECHO_PIN

// Constants
#define US_ROUNDTRIP_CM 145      // Microseconds for sound to travel 1 cm (adjust for calibration)

// Function Prototypes
void initialize_uart(void);
void uart_send_string(const char* str);
void uart_send_byte(uint8_t data);
void initialize_pins(void);
uint16_t measure_distance(void);

// UART Buffer
char uart_buffer[32];

void initialize_uart(void) {
    TRISC6 = 0;    // TX pin as output
    TRISC7 = 1;    // RX pin as input
    
    SPBRG = 129;   // Baud rate set for 9600 with 20MHz crystal
    BRGH = 1;      // High-speed baud rate
    SYNC = 0;      // Asynchronous mode
    SPEN = 1;      // Enable serial port
    TXEN = 1;      // Enable transmission
    CREN = 1;      // Enable reception
    }

void uart_send_byte(uint8_t data) {
    while (!TXIF);  // Wait for transmit buffer to be empty
    TXREG = data;   // Send the byte
    }

void uart_send_string(const char* str) {
    while (*str) {
        uart_send_byte(*str++);
        }
    }

void initialize_pins(void) {
    TRIG_PIN_DIR = 0;   // Set TRIG_PIN as output
    ECHO_PIN_DIR = 1;   // Set ECHO_PIN as input
    TRIG_PIN = 0;       // Ensure trigger pin is low initially
    }

uint16_t measure_distance(void) {
    uint16_t pulse_duration = 0;
    
    // Generate 10s trigger pulse
    TRIG_PIN = 1;
    __delay_us(10);
    TRIG_PIN = 0;
    
    // Wait for echo pin to go high
    while (!ECHO_PIN);  // Wait for rising edge
    
    // Start Timer1
    TMR1H = 0;
    TMR1L = 0;
    TMR1ON = 1;  // Start Timer1
    
    // Wait for echo pin to go low
    while (ECHO_PIN);  // Wait for falling edge
    
    // Stop Timer1
    TMR1ON = 0;
    
    // Calculate pulse duration
    pulse_duration = ((uint16_t)TMR1H << 8) | TMR1L;
    
    // Convert pulse duration to distance in cm
    return pulse_duration / US_ROUNDTRIP_CM;
    }

void main(void) {
    uint16_t distance;
    
    // Initialize peripherals
    initialize_pins();
    initialize_uart();
    
    // Timer1 configuration
    T1CON = 0x10;  // Prescaler 1:2, internal clock (Fosc/4)
    
    while (1) {
        distance = measure_distance();
        
        // Send distance over UART
        sprintf(uart_buffer, "Distance: %ucm\r\n", distance);
        uart_send_string(uart_buffer);
        
        __delay_ms(500);  // Delay before next measurement
        }
    }


 