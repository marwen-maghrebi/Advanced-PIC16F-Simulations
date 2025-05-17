
//adc.h

#ifndef ADC_H
#define ADC_H

#include "main.h"

void configure_ADC(void);
void start_ADC_conversion(void);
void wait_for_conversion(void);
int read_ADC_result(void);

#endif

