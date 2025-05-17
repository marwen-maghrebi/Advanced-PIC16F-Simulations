#include <xc.h>
#include <stdio.h>      // Add this for sprintf
#include <string.h>     // Add this for string operations

#define _XTAL_FREQ 20000000  // Define crystal frequency for delay calculations

// Configuration bits
#pragma config FOSC = HS     // High-speed oscillator
#pragma config WDTE = OFF    // Watchdog Timer disabled
#pragma config PWRTE = OFF   // Power-up Timer disabled
#pragma config BOREN = OFF   // Brown-out Reset disabled
#pragma config LVP = ON      // Low-Voltage Programming enabled
#pragma config CPD = OFF     // Data EEPROM Code Protection disabled
#pragma config WRT = OFF     // Flash Program Memory Write Protection disabled
#pragma config CP = OFF      // Flash Program Memory Code Protection disabled

// DS18B20 Pin definition - Fixed comment syntax
#define DS18B20_PIN PORTBbits.RB1
#define DS18B20_TRIS TRISBbits.TRISB1

// LCD control pins
#define RS PORTCbits.RC0
#define RW PORTCbits.RC1
#define EN PORTCbits.RC2

// Global variables for temperature - Moved to top level
static signed int raw_temp;
static float temperature;
static char temp_str[16];

// Function prototypes
void lcd_initialize(void);
void lcd_data(unsigned char data);
void lcd_string(const char *str, unsigned char len);  // Changed to char* instead of unsigned char*
void lcd_command(unsigned char cmd);

// DS18B20 Function prototypes
unsigned char ds18b20_start(void);
void ds18b20_write_bit(unsigned char bit);
void ds18b20_write_byte(unsigned char byte);
unsigned char ds18b20_read_bit(void);
unsigned char ds18b20_read_byte(void);
unsigned char ds18b20_read(signed int *raw_temp);

void main(void) {
    // Initialize LCD pins
    TRISCbits.TRISC0 = 0;
    TRISCbits.TRISC1 = 0;
    TRISCbits.TRISC2 = 0;
    
    TRISDbits.TRISD4 = 0;
    TRISDbits.TRISD5 = 0;
    TRISDbits.TRISD6 = 0;
    TRISDbits.TRISD7 = 0;
    
    // Initialize DS18B20 pin as input
    DS18B20_TRIS = 1;
    
    lcd_initialize();

    while(1) {
        if(ds18b20_read(&raw_temp)) {
            // Convert raw value to temperature
            temperature = (float)raw_temp * 0.0625;
            
            // Convert temperature to string
            sprintf(temp_str, "Temp: %2.1f C", temperature);
            
            // Display on LCD
            lcd_command(0x80);
            lcd_string(temp_str, 16);
        } else {
            lcd_command(0x80);
            lcd_string("Sensor Error!", 12);
        }
        __delay_ms(1000);  // Update every second
    }
}


// DS18B20 Functions
unsigned char ds18b20_start(void) {
    unsigned char response;
    
    DS18B20_TRIS = 0;      // Set as output
    DS18B20_PIN = 0;       // Send reset pulse
    __delay_us(500);       // Hold for 500us
    
    DS18B20_TRIS = 1;      // Set as input
    __delay_us(100);       // Wait for response
    
    response = DS18B20_PIN;// Read response
    __delay_us(400);       // Wait 400us
    
    return !response;      // Return TRUE if sensor detected
}

void ds18b20_write_bit(unsigned char bit) {
    DS18B20_TRIS = 0;      // Set as output
    DS18B20_PIN = 0;       // Pull low
    __delay_us(2);
    
    if(bit) DS18B20_PIN = 1;  // Send bit
    __delay_us(80);        // Hold for 80us
    
    DS18B20_TRIS = 1;      // Release line
    __delay_us(2);
}

void ds18b20_write_byte(unsigned char byte) {
    unsigned char i;
    for(i = 0; i < 8; i++) {
        ds18b20_write_bit(byte & 0x01);
        byte >>= 1;
    }
}

unsigned char ds18b20_read_bit(void) {
    unsigned char bit;
    
    DS18B20_TRIS = 0;      // Set as output
    DS18B20_PIN = 0;       // Pull low
    __delay_us(2);
    
    DS18B20_TRIS = 1;      // Release line
    __delay_us(5);         // Wait 5us
    
    bit = DS18B20_PIN;     // Read bit
    __delay_us(100);       // Wait 100us
    
    return bit;
}

unsigned char ds18b20_read_byte(void) {
    unsigned char i, byte = 0;
    
    for(i = 0; i < 8; i++) {
        byte >>= 1;
        if(ds18b20_read_bit())
            byte |= 0x80;
    }
    
    return byte;
}

unsigned char ds18b20_read(signed int *raw_temp_value) {
    unsigned char temp_l, temp_h;
    
    if(!ds18b20_start())
        return 0;
    
    ds18b20_write_byte(0xCC);  // Skip ROM command
    ds18b20_write_byte(0x44);  // Start conversion
    
    while(!ds18b20_read_byte());  // Wait for conversion
    
    if(!ds18b20_start())
        return 0;
    
    ds18b20_write_byte(0xCC);  // Skip ROM command
    ds18b20_write_byte(0xBE);  // Read scratchpad
    
    temp_l = ds18b20_read_byte();  // LSB
    temp_h = ds18b20_read_byte();  // MSB
    
    *raw_temp_value = (temp_h << 8) | temp_l;
    
    return 1;
}

void lcd_initialize(void) {
    lcd_command(0x02); // Return cursor to home position
    lcd_command(0x28); // Set 4-bit mode, 2-line display, 5x8 font
    lcd_command(0x06); // Set entry mode with auto-increment
    lcd_command(0x0C); // Display ON, cursor OFF
    lcd_command(0x01); // Clear display
    __delay_ms(2);     // Short delay for clear screen command
}

void lcd_data(unsigned char data) {
    RS = 1; RW = 0;       // Set to data mode and write
    PORTD = (data & 0xF0); // Send upper nibble
    EN = 1;
    __delay_ms(2);
    EN = 0;
    
    PORTD = ((data << 4) & 0xF0); // Send lower nibble
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

void lcd_string(const char *str, unsigned char len) {
    for (unsigned char i = 0; i < len; ++i) {
        lcd_data((unsigned char)str[i]); // Cast to unsigned char when sending to LCD
    }
}

void lcd_command(unsigned char cmd) {
    RS = 0; RW = 0;       // Set to command mode and write
    PORTD = (cmd & 0xF0); // Send upper nibble
    EN = 1;
    __delay_ms(2);
    EN = 0;

    PORTD = ((cmd << 4) & 0xF0); // Send lower nibble
    EN = 1;
    __delay_ms(2);
    EN = 0;
}

