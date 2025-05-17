
// I2C.c
#include "I2C.h"

void I2C_Master_Init(void) {
    SSPCON  = 0x28;
    SSPCON2 = 0x00;
    SSPSTAT = 0x00;
    SSPADD = ((_XTAL_FREQ/4)/I2C_BaudRate) - 1;
    SCL_D = 1;
    SDA_D = 1;
    }

void I2C_Master_Wait(void) {
    while ((SSPSTAT & 0x04) || (SSPCON2 & 0x1F));
    }

void I2C_Master_Start(void) {
    I2C_Master_Wait();
    SEN = 1;
    }

void I2C_Master_RepeatedStart(void) {
    I2C_Master_Wait();
    RSEN = 1;
    }

void I2C_Master_Stop(void) {
    I2C_Master_Wait();
    PEN = 1;
    }

void I2C_ACK(void) {
    ACKDT = 0;
    I2C_Master_Wait();
    ACKEN = 1;
    }

void I2C_NACK(void) {
    ACKDT = 1;
    I2C_Master_Wait();
    ACKEN = 1;
    }

unsigned char I2C_Master_Write(unsigned char data) {
    I2C_Master_Wait();
    SSPBUF = data;
    while(!SSPIF);
    SSPIF = 0;
    return ACKSTAT;
    }

unsigned char I2C_Read_Byte(void) {
    I2C_Master_Wait();
    RCEN = 1;
    while(!SSPIF);
    SSPIF = 0;
    I2C_Master_Wait();
    return SSPBUF;
    }