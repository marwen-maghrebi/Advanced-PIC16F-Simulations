
#include <xc.h>
#include <stdio.h>
#include "uart.h"

// Configuration bits
#pragma config FOSC = HS        // High Speed Crystal
#pragma config WDTE = OFF       // Watchdog Timer disabled
#pragma config PWRTE = ON       // Power-up Timer enabled
#pragma config BOREN = ON       // Brown-out Reset enabled
#pragma config LVP = OFF        // Low Voltage Programming disabled
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection disabled
#pragma config WRT = OFF        // Flash Program Memory Write disabled
#pragma config CP = OFF         // Flash Program Memory Code Protection disabled

#define _XTAL_FREQ 8000000     // 8MHz Crystal

// DHT11 pin definitions
#define DHT11_PIN      RB0
#define DHT11_PIN_DIR  TRISB0

// Global variables
char Temperature[] = "Temp = 00.0 C";
char Humidity[]    = "RH   = 00.0 %";
unsigned char T_Byte1, T_Byte2, RH_Byte1, RH_Byte2, CheckSum;

// Send start signal to the sensor
void Start_Signal(void) {
    DHT11_PIN_DIR = 0;     // Configure as output
    DHT11_PIN = 0;         // Pull data line low
    __delay_ms(25);        // Wait 25ms
    DHT11_PIN = 1;         // Release data line
    __delay_us(30);        // Wait 30us
    DHT11_PIN_DIR = 1;     // Configure as input
    }

// Check sensor response
unsigned char Check_Response(void) {
    TMR1H = 0;                  // Reset Timer1
    TMR1L = 0;
    TMR1ON = 1;                 // Enable Timer1 module
    
    while(!DHT11_PIN && TMR1L < 100);  // Wait for DHT11_PIN to go high (80s low time)
    
    if(TMR1L > 99)              // If response time > 99s
        return 0;               // Return 0 (response error)
    
    TMR1H = 0;                  // Reset Timer1
    TMR1L = 0;
    
    while(DHT11_PIN && TMR1L < 100);   // Wait for DHT11_PIN to go low (80s high time)
    
    if(TMR1L > 99)              // If response time > 99s
        return 0;               // Return 0 (response error)
    
    return 1;                   // Return 1 (response OK)
    }

// Read data from sensor
unsigned char Read_Data(unsigned char* dht_data) {
    *dht_data = 0;
    
    for(char i = 0; i < 8; i++) {
        TMR1H = 0;              // Reset Timer1
        TMR1L = 0;
        
        while(!DHT11_PIN)       // Wait for rising edge
            if(TMR1L > 100)     // Timeout if low time > 100s
                return 1;
        
        TMR1H = 0;              // Reset Timer1
        TMR1L = 0;
        
        while(DHT11_PIN)        // Wait for falling edge
            if(TMR1L > 100)     // Timeout if high time > 100s
                return 1;
        
        if(TMR1L > 50)          // If high time > 50s = bit is 1
            *dht_data |= (1 << (7 - i));
        }
    
    return 0;                   // Return 0 (data read OK)
    }

void main(void) {
    // Initialize ports
    TRISB = 0b00000001;    // RB0 as input for DHT11
    TRISC = 0b10000000;    // Set RC6 as output for UART TX
    ADCON1 = 0x06;         // Configure PORTA pins as digital I/O
    
    // Timer1 configuration - 1:2 prescaler (Timer1 clock = 1MHz at 8MHz crystal)
    T1CON = 0x10;
    TMR1H = 0;
    TMR1L = 0;
    
    // Initialize UART
    UART_TX_Init();
    __delay_ms(1000);      // Initial delay
    
    while(1) {
        Start_Signal();     // Send start signal
        
        if(Check_Response()) {  // If sensor responds
            // Read all data bytes
            if(Read_Data(&RH_Byte1) || Read_Data(&RH_Byte2) ||
                    Read_Data(&T_Byte1) || Read_Data(&T_Byte2) ||
                    Read_Data(&CheckSum)) {
                UART_Write_Text("Sensor read timeout!\n\r");
                }
            else {  // If no timeout occurred
                if(CheckSum == ((RH_Byte1 + RH_Byte2 + T_Byte1 + T_Byte2) & 0xFF)) {
                    // Update message strings
                    Temperature[7] = T_Byte1/10 + '0';
                    Temperature[8] = T_Byte1%10 + '0';
                    Temperature[10] = T_Byte2/10 + '0';
                    Humidity[7] = RH_Byte1/10 + '0';
                    Humidity[8] = RH_Byte1%10 + '0';
                    Humidity[10] = RH_Byte2/10 + '0';
                    
                    // Send via UART
                    UART_Write_Text(Temperature);
                    UART_Write_Text("\n\r");
                    UART_Write_Text(Humidity);
                    UART_Write_Text("\n\r");
                    }
                else {
                    UART_Write_Text("Checksum Error!\n\r");
                    }
                }
            }
        else {
            UART_Write_Text("No response from sensor\n\r");
            }
        
        TMR1ON = 0;        // Disable Timer1
        __delay_ms(1000);  // Wait 1 second before next reading
    }
}

