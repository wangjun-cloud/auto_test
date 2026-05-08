#ifndef __DRV_ADC_H
#define __DRV_ADC_H

#include "stm32f10x.h"

#define ADC_CH_PB0  ADC_Channel_8
#define ADC_CH_PB1  ADC_Channel_9

void drv_adc_init(void);
uint16_t drv_adc_read(uint8_t channel);
uint16_t drv_adc_to_voltage(uint16_t adc_value);

#endif
