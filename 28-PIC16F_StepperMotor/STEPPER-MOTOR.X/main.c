
#include <xc.h>
#include <stdint.h>

// Configuration bits
#pragma config FOSC = XT        // Oscillator Selection bits
#pragma config WDTE = OFF       // Watchdog Timer Enable bit
#pragma config PWRTE = ON       // Power-up Timer Enable bit
#pragma config BOREN = ON       // Brown-out Reset Enable bit
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits
#pragma config CP = OFF         // Flash Program Memory Code Protection bit

#define _XTAL_FREQ 8000000      // Define crystal frequency for delay
#define out PORTD               // Stepper motor connected to PORTD

// Turn sequence for stepper motor
const unsigned char turn[] = {0x02, 0x06, 0x04, 0x0C, 0x08, 0x09, 0x01, 0x03};

// UART Functions (provided)
void UART_TX_Init(void)
{
    // Set baud rate to 9600 bps
    BRGH = 1;       // High-speed baud rate
    SPBRG = 51;     // Set SPBRG for 9600 bps with 8 MHz clock
    
    // Enable Asynchronous Serial Port
    SYNC = 0;
    SPEN = 1;
    
    // Set RX-TX Pins to be in UART mode
    TRISC6 = 0;     // TX pin (output)
    TRISC7 = 1;     // RX pin (input)
    
    // Enable UART Transmission
    TXEN = 1;
    // Enable UART Reception
    CREN = 1;
}

uint8_t UART_TX_Empty()
{
    // Check if the output (transmitter) buffer is empty
    return TRMT;
}

void UART_Write(uint8_t data)
{
    // Wait until the transmitter buffer is empty
    while (!UART_TX_Empty());
    
    // Write data to the transmitter buffer
    TXREG = data;
}

void UART_Write_Text(const char* text)
{
    while (*text != '\0') {
        UART_Write(*text++);
    }
}

uint8_t UART_Read(void)
{
    // Wait until data is received
    while (!RCIF);
    
    // Read and return the received data
    return RCREG;
}

void UART_Read_Text(char* buffer, uint8_t max_length)
{
    uint8_t i = 0;
    char received_char;
    
    // Receive characters until Enter key is pressed or buffer is full
    do {
        received_char = UART_Read();
        if (received_char != '\r' && received_char != '\n') {
            buffer[i++] = received_char;
            }
        } while (received_char != '\r' && received_char != '\n' && i < max_length - 1);
        
        // Null-terminate the string
        buffer[i] = '\0';
}

void main(void) {
unsigned char i = 0;

// Configure Ports
TRISD = 0x00;              // Set PORTD as output
PORTD = 0xFF;              // Initialize PORTD to high
UART_TX_Init();            // Initialize UART
char command = 'S';  // Default state is stopped
out = 0x00;              // Stop motor
UART_Write_Text("Stepper Motor Control Initialized\n\r");
UART_Write_Text("Enter 'F' to move forward, 'B' to move backward, or 'S' to stop:\n\r");

while (1) {
    // Check if a new command is received
    if (PIR1bits.RCIF) {
        char new_command = UART_Read();  // Read the new command

        if (new_command == 'F' || new_command == 'f') {
        UART_Write_Text("\n\rMotor Moving Forward\n\r");
        command = 'F';  // Update the state to forward
        } else if (new_command == 'B' || new_command == 'b') {
        UART_Write_Text("\n\rMotor Moving Backward\n\r");
        command = 'B';  // Update the state to backward
        } else if (new_command == 'S' || new_command == 's') {
        UART_Write_Text("\n\rMotor Stopped\n\r");
        command = 'S';  // Update the state to stopped
        } else {
        UART_Write_Text("\n\rInvalid Command. Maintaining Last State.\n\r");
        // Keep the current command unchanged
        }
    }
    // Act based on the current valid command
    if (command == 'F') {
    i = (i < 7) ? i + 1 : 0;  // Increment index with wraparound
    out = turn[i];           // Output step sequence
    __delay_ms(50);          // Step delay
    } else if (command == 'B') {
    i = (i > 0) ? i - 1 : 7;  // Decrement index with wraparound
    out = turn[i];           // Output step sequence
    __delay_ms(50);          // Step delay
    } else if (command == 'S') {
        out = 0x00;              // Stop motor
    }
}}


