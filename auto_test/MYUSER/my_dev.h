#ifndef MY_DEV_H
#define MY_DEV_H

#include "main.h"

typedef enum {
    STATE_INIT = 0,
    STATE_IDLE,
    STATE_RUN
} eSoftState;

typedef enum {
    RUN_IDLE_WAIT = 0,
    RUN_PWM_PB6_ADC,
    RUN_SWITCH_TO_PB7,
    RUN_PWM_PB7_ADC,
    RUN_SWITCH_TO_PB6,
    RUN_PWM_PB6_ADC2,
    RUN_DONE_WAIT
} eRunSubState;

typedef struct {
    eSoftState  soft_state;
    eRunSubState run_sub;
    uint32_t    state_tick;
    uint16_t    adc_val[2];
    uint8_t     adc_ok[2];
} tMyDev;

extern tMyDev TMyDev;

uint32_t get_local_absolute_time(void);
void my_dev_systick_inc(void);
void my_dev_init(void);
void my_dev_process(void);

/* PWM interface */
void my_pwm_init(void);
void my_pwm_start_pb6(void);
void my_pwm_stop_pb6(void);
void my_pwm_start_pb7(void);
void my_pwm_stop_pb7(void);
void my_pwm_stop_all(void);

/* ADC interface */
void my_adc_init(void);
uint16_t my_adc_read_pb0(void);
uint16_t my_adc_read_pb1(void);

#endif
