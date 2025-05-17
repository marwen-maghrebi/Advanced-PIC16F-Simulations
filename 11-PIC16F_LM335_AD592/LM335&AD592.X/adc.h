
#ifndef ADC_H
#define ADC_H

#include "main.h"

// Function Prototypes
void configure_ADC(void);
void select_ADC_channel(unsigned char channel) ;
void start_ADC_conversion(void);
void wait_for_conversion(void);
uint16_t read_ADC_result(void);

#endif /* ADC_H */


