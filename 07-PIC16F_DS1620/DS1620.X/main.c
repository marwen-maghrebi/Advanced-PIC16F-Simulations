
#include <xc.h>
#include <stdio.h>
#include <string.h>

#define _XTAL_FREQ 20000000

// Configuration bits
#pragma config FOSC = HS     // High-speed oscillator
#pragma config WDTE = OFF    // Watchdog Timer disabled
#pragma config PWRTE = OFF   // Power-up Timer disabled
#pragma config BOREN = OFF   // Brown-out Reset disabled
#pragma config LVP = ON      // Low-Voltage Programming enabled
#pragma config CPD = OFF     // Data EEPROM Code Protection disabled
#pragma config WRT = OFF     // Flash Program Memory Write Protection disabled
#pragma config CP = OFF      // Flash Program Memory Code Protection disabled

// LCD control pins
#define RS PORTCbits.RC3
#define RW PORTCbits.RC4
#define EN PORTCbits.RC5

// DS1620 pins
#define DQ PORTCbits.RC0
#define DQ_TRIS TRISCbits.TRISC0
#define CLK PORTCbits.RC1
#define RST PORTCbits.RC2

// DS1620 Commands
#define START_CONVERT  0xEE
#define STOP_CONVERT   0x22
#define READ_TEMP      0xAA
#define WRITE_CONFIG   0x0C
#define READ_CONFIG    0xAC

// Global variables
static char temp_str[16];

// Function prototypes
void lcd_initialize(void);
void lcd_data(unsigned char data);
void lcd_string(const char *str, unsigned char len);
void lcd_command(unsigned char cmd);
void write_data(unsigned char data);
unsigned int read_data(void);
void start_transfer(void);
void end_transfer(void);
void ds1620_init(void);
signed int read_temperature(void);

// DS1620 Functions
void write_data(unsigned char data) {
    unsigned char i;
    DQ_TRIS = 0;  // Set DQ as output
    
    for(i = 0; i < 8; i++) {
        CLK = 0;
        DQ = (data >> i) & 0x01;  // LSB first
        CLK = 1;
        __delay_us(1);
    }
}

unsigned int read_data(void) {
    unsigned char i;
    unsigned int data = 0;
    
    DQ_TRIS = 1;  // Set DQ as input
    
    for(i = 0; i < 9; i++) {  // Read 9 bits for temperature
        CLK = 0;
        __delay_us(1);
        data |= ((unsigned int)(DQ) << i);  // LSB first
        CLK = 1;
        __delay_us(1);
        }
    
    DQ_TRIS = 0;  // Set back to output
    return data;
}

void start_transfer(void) {
    CLK = 0;
    RST = 1;
    __delay_us(1);
}

void end_transfer(void) {
    RST = 0;
    __delay_us(1);
}

void ds1620_init(void) {
    // Configure DS1620
    start_transfer();
    write_data(WRITE_CONFIG);
    write_data(0x02);  // CPU mode, continuous conversion
    end_transfer();
    __delay_ms(50);    // Allow time for writing to config register
    
    // Start continuous conversion
    start_transfer();
    write_data(START_CONVERT);
    end_transfer();
    __delay_ms(750);   // Maximum conversion time
}

signed int read_temperature(void) {
    unsigned int raw_temp;
    
    start_transfer();
    write_data(READ_TEMP);
    raw_temp = read_data();
    end_transfer();
    
    // Check if the 9th bit is set for two?s complement
    if (raw_temp & 0x100) {  // 0x100 is 9th bit in 9-bit number
        raw_temp |= ~0x1FF;  // Sign-extend for two?s complement
        }
    
    return raw_temp;  // Now return the correct signed value
}

// LCD Functions
void lcd_initialize(void) {
    __delay_ms(15);    // Wait for LCD to power up
    
    // Initialize in 4-bit mode
    lcd_command(0x03);
    __delay_ms(5);
    lcd_command(0x03);
    __delay_us(150);
    lcd_command(0x03);
    lcd_command(0x02);
    
    lcd_command(0x28); // 4-bit mode, 2 lines, 5x8 font
    lcd_command(0x0C); // Display ON, cursor OFF
    lcd_command(0x06); // Auto-increment cursor
    lcd_command(0x01); // Clear display
    __delay_ms(2);     // Clear display command needs longer delay
}

void lcd_data(unsigned char data) {
    RS = 1;
    RW = 0;
    
    // Send upper nibble
    PORTD &= 0x0F;
    PORTD |= (data & 0xF0);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(100);
    
    // Send lower nibble
    PORTD &= 0x0F;
    PORTD |= ((data << 4) & 0xF0);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(100);
}

void lcd_string(const char *str, unsigned char len) {
    for (unsigned char i = 0; i < len; i++) {
        lcd_data(str[i]);
    }
}

void lcd_command(unsigned char cmd) {
    RS = 0;
    RW = 0;
    
    // Send upper nibble
    PORTD &= 0x0F;
    PORTD |= (cmd & 0xF0);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(100);
    
    // Send lower nibble
    PORTD &= 0x0F;
    PORTD |= ((cmd << 4) & 0xF0);
    EN = 1;
    __delay_us(1);
    EN = 0;
    __delay_us(100);
}

void main(void) {
    signed int temperature;
    
    // Initialize pins
    TRISCbits.TRISC1 = 0;  // CLK
    TRISCbits.TRISC2 = 0;  // RST
    TRISCbits.TRISC3 = 0;  // LCD RS
    TRISCbits.TRISC4 = 0;  // LCD RW
    TRISCbits.TRISC5 = 0;  // LCD EN
    
    // Set D4-D7 as outputs for LCD
    TRISD &= 0x0F;
    PORTD &= 0x0F;
    
    // Initial states
    CLK = 0;
    RST = 0;
    
    __delay_ms(100);  // Power-up delay
    
    // Initialize LCD
    lcd_initialize();
    
    // Initialize DS1620
    ds1620_init();
    
    
    // Display "DS1620 SENSOR" on the first row
    lcd_command(0x80);  // Move cursor to the first row, first column
    lcd_string("DS1620 SENSOR", 13);  // Display text "DS1620 SENSOR"
    
    // Main loop
    while(1) {
        // Read temperature
        temperature = read_temperature();
        
        // Convert temperature to string with proper formatting
        sprintf(temp_str, "Temp: %d.%dC",
                temperature / 2,           // Integer part
                (temperature & 1) ? 5 : 0  // Decimal part (0.5 resolution)
                );
        
        // Display temperature on the second row
        lcd_command(0xC0);  // Move cursor to the second row, first column
        lcd_string(temp_str, strlen(temp_str));
        
        __delay_ms(1000);  // Update every second
    }
}

