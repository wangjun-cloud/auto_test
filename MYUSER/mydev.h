#ifndef __MYDEV_H
#define __MYDEV_H

#include "stm32f10x.h"

#define TIMEOUT_INIT_TO_IDLE   10000
#define TIMEOUT_IDLE_TO_RUN    10000
#define TIMEOUT_RUN_TO_IDLE    10000
#define TIMEOUT_PWM_STABLE     50
#define ADC_REF_VOLTAGE        3300
#define ADC_RESOLUTION         4096
#define PWM_FREQUENCY          1000
#define PWM_DUTY_DEFAULT       50

typedef enum {
    SOFT_INIT = 0, SOFT_IDLE, SOFT_RUN,
    SOFT_RUN_STEP1, SOFT_RUN_STEP2, SOFT_RUN_STEP3,
    SOFT_RUN_DONE
} soft_state_enum;

typedef enum {
    RUN_SUB_IDLE = 0, RUN_SUB_PWM_ON,
    RUN_SUB_WAIT_STABLE, RUN_SUB_ADC_SAMPLE,
    RUN_SUB_CHECK, RUN_SUB_PWM_OFF
} run_sub_state_enum;

typedef enum {
    ADC_RESULT_NONE = 0, ADC_RESULT_NORMAL, ADC_RESULT_ABNORMAL
} adc_result_enum;

typedef struct {
    soft_state_enum    soft_state;
    run_sub_state_enum run_sub_state;
    uint32_t           state_timeout;
    uint32_t           sub_timeout;
    uint16_t           adc_value_pb0;
    uint16_t           adc_value_pb1;
    uint16_t           adc_voltage_pb0;
    uint16_t           adc_voltage_pb1;
    adc_result_enum    adc_result_pb0;
    adc_result_enum    adc_result_pb1;
    uint32_t           run_count;
    uint32_t           err_count;
} tMyDev;

extern tMyDev TMyDev;
void mydev_init(tMyDev *pMyDev);

#endif
