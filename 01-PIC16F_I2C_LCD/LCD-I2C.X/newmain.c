/* File: main.c
 * Author: Marwen Maghrebi
 * Description: Advanced LCD Interface for PIC16F877A Using PCF8574
 * This code demonstrates an enhanced interface for LCD (Liquid Crystal Display) with a PIC microcontroller, specifically the PIC16F877A, 
 *utilizing the PCF8574 I2C I/O expander. It initializes the LCD, clears the screen, and prints messages. Additionally, it provides functions 
 *to control the cursor position, send commands or data to the LCD, and manage the backlight.
 */
 
 /* File: main.c */

// Include necessary header files
#include <xc.h>

// Define constants
#define _XTAL_FREQ             16000000

#define I2C_BaudRate           100000
#define SCL_D                  TRISC3
#define SDA_D                  TRISC4

#define LCD_BACKLIGHT          0x08
#define LCD_NOBACKLIGHT        0x00
#define LCD_FIRST_ROW          0x80
#define LCD_SECOND_ROW         0xC0
#define LCD_THIRD_ROW          0x94
#define LCD_FOURTH_ROW         0xD4
#define LCD_CLEAR              0x01
#define LCD_RETURN_HOME        0x02
#define LCD_ENTRY_MODE_SET     0x04
#define LCD_CURSOR_OFF         0x0C
#define LCD_UNDERLINE_ON       0x0E
#define LCD_BLINK_CURSOR_ON    0x0F
#define LCD_MOVE_CURSOR_LEFT   0x10
#define LCD_MOVE_CURSOR_RIGHT  0x14
#define LCD_TURN_ON            0x0C
#define LCD_TURN_OFF           0x08
#define LCD_SHIFT_LEFT         0x18
#define LCD_SHIFT_RIGHT        0x1E
#define LCD_TYPE               2       // 0 -> 5x7 | 1 -> 5x10 | 2 -> 2 lines

// Function prototypes
void main(void);
void I2C_Master_Init(void);
void I2C_Master_Wait(void);
void I2C_Master_Start(void);
void I2C_Master_RepeatedStart(void);
void I2C_Master_Stop(void);
void I2C_ACK(void);
void I2C_NACK(void);
unsigned char I2C_Master_Write(unsigned char data);
unsigned char I2C_Read_Byte(void);
void LCD_Init(unsigned char I2C_Add);
void IO_Expander_Write(unsigned char Data);
void LCD_Write_4Bit(unsigned char Nibble);
void LCD_CMD(unsigned char CMD);
void LCD_Set_Cursor(unsigned char ROW, unsigned char COL);
void LCD_Print_Char(char Data);
void LCD_Print_String(char* Str);
void Backlight(void);
void noBacklight(void);
void LCD_SR(void);
void LCD_SL(void);
void LCD_Clear(void);

// PIC16F877A Configuration Bit Settings
#pragma config FOSC = HS        // Oscillator Selection bits (HS oscillator)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = OFF      // Power-up Timer Enable bit (PWRT disabled)
#pragma config BOREN = OFF      // Brown-out Reset Enable bit (BOR disabled)
#pragma config LVP = OFF        // Low-Voltage (Single-Supply) In-Circuit Serial Programming Enable bit (RB3 is digital I/O, HV on MCLR must be used for programming)
#pragma config CPD = OFF        // Data EEPROM Memory Code Protection bit (Data EEPROM code protection off)
#pragma config WRT = OFF        // Flash Program Memory Write Enable bits (Write protection off; all program memory may be written to by EECON control)
#pragma config CP = OFF         // Flash Program Memory Code Protection bit (Code protection off)

unsigned char i2c_add, BackLight_State = LCD_BACKLIGHT;
unsigned char RS;

void main(void) {
    
    // Initialize I2C and LCD
    I2C_Master_Init();
    LCD_Init(0x4E);    // Initialize LCD module with I2C address = 0x4E
 
    // Write strings to LCD
    LCD_Set_Cursor(1, 1);
    LCD_Print_String("  THE EMBEDDED  ");
    LCD_Set_Cursor(2, 1);  
    LCD_Print_String("     THINGS     ");
    __delay_ms(2500);

    // Animation loop
    while(1)
    {
       LCD_SR();
       __delay_ms(350); 
       LCD_SR();
       __delay_ms(350); 
       LCD_SL();
       __delay_ms(350);
       LCD_SL();
       __delay_ms(350);
    }
    return;
}

void I2C_Master_Init(void)
{
    SSPCON  = 0x28;
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
    SCL_D = 1;
    SDA_D = 1; 
}

void I2C_Master_Wait(void)
{
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
}

void I2C_Master_Start(void)
{
    I2C_Master_Wait();
    SEN = 1;
}

void I2C_Master_RepeatedStart(void)
{
    I2C_Master_Wait();
    RSEN = 1;
}

void I2C_Master_Stop(void)
{
    I2C_Master_Wait();
    PEN = 1;
}

void I2C_ACK(void)
{
    ACKDT = 0;          // 0 -> ACK
    I2C_Master_Wait();
    ACKEN = 1;          // Send ACK
}

void I2C_NACK(void)
{
    ACKDT = 1;          // 1 -> NACK
    I2C_Master_Wait();
    ACKEN = 1;          // Send NACK
}

unsigned char I2C_Master_Write(unsigned char data)
{
    I2C_Master_Wait();
    SSPBUF = data;
    while(!SSPIF);      // Wait Until Completion
    SSPIF = 0;
    return ACKSTAT;
}

unsigned char I2C_Read_Byte(void)
{
    // Receive & Return A Byte
    I2C_Master_Wait();
    RCEN = 1;           // Enable & Start Reception
    while(!SSPIF);      // Wait Until Completion
    SSPIF = 0;          // Clear The Interrupt Flag Bit
    I2C_Master_Wait();
    return SSPBUF;      // Return The Received Byte
}

void LCD_Init(unsigned char I2C_Add) 
{
    i2c_add = I2C_Add;
    IO_Expander_Write(0x00);
    __delay_ms(30);
    LCD_CMD(0x03);
    __delay_ms(5);
    LCD_CMD(0x03);
    __delay_ms(5);
    LCD_CMD(0x03);
    __delay_ms(5);
    LCD_CMD(LCD_RETURN_HOME);
    __delay_ms(5);
    LCD_CMD(0x20 | (LCD_TYPE << 2));
    __delay_ms(50);
    LCD_CMD(LCD_TURN_ON);
    __delay_ms(50);
    LCD_CMD(LCD_CLEAR);
    __delay_ms(50);
    LCD_CMD(LCD_ENTRY_MODE_SET | LCD_RETURN_HOME);
    __delay_ms(50);
}

void IO_Expander_Write(unsigned char Data) 
{
    I2C_Master_Start();
    I2C_Master_Write(i2c_add);
    I2C_Master_Write(Data | BackLight_State);
    I2C_Master_Stop();
}

void LCD_Write_4Bit(unsigned char Nibble) 
{
    // Get The RS Value To LSB OF Data  
    Nibble |= RS;
    IO_Expander_Write(Nibble | 0x04);
    IO_Expander_Write(Nibble & 0xFB);
    __delay_us(50);
}

void LCD_CMD(unsigned char CMD) 
{
    RS = 0; // Command Register Select
    LCD_Write_4Bit(CMD & 0xF0);
    LCD_Write_4Bit((CMD << 4) & 0xF0);
}

void LCD_Print_Char(char Data)
{
    RS = 1;  // Data Register Select
    LCD_Write_4Bit(Data & 0xF0);
    LCD_Write_4Bit((Data << 4) & 0xF0);
}

void LCD_Print_String(char* Str)
{
    for(int i=0; Str[i]!='\0'; i++)
       LCD_Print_Char(Str[i]); 
}

void LCD_Set_Cursor(unsigned char ROW, unsigned char COL) 
{    
    switch(ROW) 
    {
        case 2:
            LCD_CMD(0xC0 + COL-1);
            break;
        case 3:
            LCD_CMD(0x94 + COL-1);
            break;
        case 4:
            LCD_CMD(0xD4 + COL-1);
            break;
        default:
            LCD_CMD(0x80 + COL-1);
    }
}

void Backlight(void) 
{
    BackLight_State = LCD_BACKLIGHT;
    IO_Expander_Write(0);
}

void noBacklight(void) 
{
    BackLight_State = LCD_NOBACKLIGHT;
    IO_Expander_Write(0);
}

void LCD_SL(void)
{
    LCD_CMD(0x18);
    __delay_us(40);
}

void LCD_SR(void)
{
    LCD_CMD(0x1C);
    __delay_us(40);
}

void LCD_Clear(void)
{
    LCD_CMD(0x01); 
    __delay_us(40);
}
