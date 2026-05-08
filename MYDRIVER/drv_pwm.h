#ifndef __DRV_PWM_H
#define __DRV_PWM_H

#include "stm32f10x.h"

#define PWM_CH_PB6    0
#define PWM_CH_PB7    1

void drv_pwm_init(void);
void drv_pwm_on(uint8_t channel, uint8_t duty);
void drv_pwm_off(uint8_t channel);

#endif
