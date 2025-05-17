
#include "LM75.h"

//------------------------------------------------------------------------------
// Initializes the LM75 temperature sensor
void LM75_init(void)
{
    I2C_Master_Init();
    }
//------------------------------------------------------------------------------

// Reads temperature or any other register specified by regAddress
float LM75_read(char regAddress)
{
    signed int MSB, LSB;
    
    I2C_Master_Start();
    I2C_Master_Write(LM75_ADDRESS);          // Write mode
    I2C_Master_Write(regAddress);             // Register to read from
    I2C_Master_RepeatedStart();
    I2C_Master_Write(LM75_ADDRESS + 1);      // Read mode
    MSB = I2C_Read_Byte();                    // Read MSB
    I2C_ACK();
    LSB = I2C_Read_Byte();                    // Read LSB
    I2C_NACK();
    I2C_Master_Stop();
    
    return toFloat((MSB << 8) + LSB);
    }
//------------------------------------------------------------------------------

// Sets the LM75 configuration register
void LM75_setConfig(char data)
{
    I2C_Master_Start();
    I2C_Master_Write(LM75_ADDRESS);               // Write mode
    I2C_Master_Write(CONFIGURATION_ADDRESS);      // Config register
    I2C_Master_Write(data);                       // Config data
    I2C_Master_Stop();
    }
//------------------------------------------------------------------------------

// Reads the LM75 configuration register
char LM75_readConfig(void)
{
    I2C_Master_Start();
    I2C_Master_Write(LM75_ADDRESS);               // Write mode
    I2C_Master_Write(CONFIGURATION_ADDRESS);      // Config register
    I2C_Master_RepeatedStart();
    I2C_Master_Write(LM75_ADDRESS + 1);           // Read mode
    char config = I2C_Read_Byte();                // Read config
    I2C_NACK();
    I2C_Master_Stop();
    
    return config;
    }
//------------------------------------------------------------------------------

// Writes data to a specific register (such as THYST or TOS) on the LM75
void LM75_set(char regAddress, signed int data)
{
    I2C_Master_Start();
    I2C_Master_Write(LM75_ADDRESS);          // Write mode
    I2C_Master_Write(regAddress);            // Register to write to
    I2C_Master_Write(data >> 8);             // Write MSB
    I2C_Master_Write(data & 0xFF);           // Write LSB
    I2C_Master_Stop();
    }
//------------------------------------------------------------------------------

// Converts raw temperature data to float
float toFloat(signed int tempr)
{
    float result = (float)(tempr >> 8);    // Extract integer part
    char isfraction = tempr & FRACTION_FLAG; // Check fraction flag
    
    if (isfraction)
        {
        result += 0.5;
        }
    
    return result;
    }
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
// Sets shutdown mode
void LM75_setShutdownMode(char enable)
{
    char config = LM75_readConfig();
    if(enable)
        config |= SHUTDOWN_MODE;    // Set shutdown bit
    else
        config &= ~SHUTDOWN_MODE;   // Clear shutdown bit
    LM75_setConfig(config);
    }
//------------------------------------------------------------------------------
// Sets device mode (Comparator/Interrupt)
void LM75_setDeviceMode(char interruptMode)
{
    char config = LM75_readConfig();
    if(interruptMode)
        config |= INTERRUPT_MODE;    // Set interrupt mode
    else
        config &= ~INTERRUPT_MODE;   // Clear for comparator mode
    LM75_setConfig(config);
    }
//------------------------------------------------------------------------------
// Sets O.S. output polarity
void LM75_setOSPolarity(char activeHigh)
{
    char config = LM75_readConfig();
    if(activeHigh)
        config |= OS_POLARITY;    // Set for active high
    else
        config &= ~OS_POLARITY;   // Clear for active low
    LM75_setConfig(config);
    }
//------------------------------------------------------------------------------
// Sets fault queue size (1, 2, 4, or 6 faults)
void LM75_setFaultQueue(char faultCount)
{
    char config = LM75_readConfig();
    config &= ~FAULT_QUEUE_MASK;    // Clear fault queue bits
    
    switch(faultCount) {
        case 1:
        config |= FAULT_QUEUE_1;
        break;
        case 2:
        config |= FAULT_QUEUE_2;
        break;
        case 4:
        config |= FAULT_QUEUE_4;
        break;
        case 6:
        config |= FAULT_QUEUE_6;
        break;
        default:
        config |= FAULT_QUEUE_1; // Default to 1 fault
    }
LM75_setConfig(config);
}
//------------------------------------------------------------------------------
// Read functions for configuration settings
char LM75_isShutdown(void)
{
    return (LM75_readConfig() & SHUTDOWN_MODE) ? 1 : 0;
    }

char LM75_getDeviceMode(void)
{
    return (LM75_readConfig() & INTERRUPT_MODE) ? 1 : 0;
    }

char LM75_getOSPolarity(void)
{
    return (LM75_readConfig() & OS_POLARITY) ? 1 : 0;
    char config = LM75_readConfig();
    
    }

char LM75_getFaultQueue(void)
{
    return (LM75_readConfig() & FAULT_QUEUE_MASK) >> 3;
    }

// Reads the hysteresis temperature (THYST register) as a float value
float LM75_getHysteresisTemperature(void)
{
    unsigned char rawTemp = LM75_read(THYST_ADDRESS); // Assuming this returns the correct raw value
    if (rawTemp == -1) return LM75A_INVALID_TEMPERATURE;
    
    // Convert raw value to temperature (assuming the conversion is correct here)
    return (float)rawTemp;
    }

// Reads the over-temperature shutdown threshold (TOS register) as a float value
float LM75_getOSTripTemperature(void)
{
    unsigned char rawTemp = LM75_read(TOS_ADDRESS); // Assuming this returns the correct raw value
    if (rawTemp == -1) return LM75A_INVALID_TEMPERATURE;
    
    // Convert raw value to temperature (assuming the conversion is correct here)
    return (float)rawTemp;
    }


// Sets the hysteresis temperature threshold (THYST register) from a float value
void LM75_setHysteresisTemperature(float temperature)
{
    signed int data = (signed int)(temperature * 256);
    LM75_set(THYST_ADDRESS, data);
 }

// Sets the over-temperature shutdown threshold (TOS register) from a float value
void LM75_setOSTripTemperature(float temperature)
{
    signed int data = (signed int)(temperature * 256);
    LM75_set(TOS_ADDRESS, data);
}


