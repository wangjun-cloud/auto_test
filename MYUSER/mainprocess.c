#include "mainprocess.h"
#include "drv_pwm.h"
#include "drv_adc.h"
#include "my_rtt_debug.h"

extern uint32_t get_local_absolute_time(void);

static void check_voltage(tMyDev *p, uint8_t ch)
{
    uint16_t vol = (ch == 0) ? p->adc_voltage_pb0 : p->adc_voltage_pb1;
    adc_result_enum *r = (ch == 0) ? &p->adc_result_pb0 : &p->adc_result_pb1;
    if (vol <= ADC_REF_VOLTAGE) *r = ADC_RESULT_NORMAL;
    else { *r = ADC_RESULT_ABNORMAL; p->err_count++; }
}

void mainprocess_parse(tMyDev *p)
{
    uint32_t now = get_local_absolute_time();

    switch (p->soft_state) {

    case SOFT_INIT:
        rtt_print_msg("[SYS] INIT - hardware init\r\n");
        drv_pwm_init();
        drv_adc_init();
        mydev_init(p);
        p->soft_state = SOFT_IDLE;
        p->state_timeout = now + TIMEOUT_INIT_TO_IDLE;
        rtt_print_msg("[SYS] Init done, 10s to IDLE\r\n");
        break;

    case SOFT_IDLE:
        if (now >= p->state_timeout) {
            p->soft_state = SOFT_RUN;
            p->run_sub_state = RUN_SUB_IDLE;
            rtt_print_msg("[SYS] IDLE timeout -> RUN\r\n");
        }
        break;

    case SOFT_RUN:
        p->soft_state = SOFT_RUN_STEP1;
        p->run_sub_state = RUN_SUB_PWM_ON;
        p->run_count++;
        rtt_print_msg("[SYS] === RUN cycle start ===\r\n");
        break;

    case SOFT_RUN_STEP1:
        switch (p->run_sub_state) {
        case RUN_SUB_PWM_ON:
            drv_pwm_on(PWM_CH_PB6, PWM_DUTY_DEFAULT);
            rtt_print_pwm(0, 1);
            p->run_sub_state = RUN_SUB_WAIT_STABLE;
            p->sub_timeout = now + TIMEOUT_PWM_STABLE;
            break;
        case RUN_SUB_WAIT_STABLE:
            if (now >= p->sub_timeout) p->run_sub_state = RUN_SUB_ADC_SAMPLE;
            break;
        case RUN_SUB_ADC_SAMPLE:
            p->adc_value_pb0 = drv_adc_read(ADC_CH_PB0);
            p->adc_voltage_pb0 = drv_adc_to_voltage(p->adc_value_pb0);
            check_voltage(p, 0);
            rtt_print_adc(0, p->adc_value_pb0, p->adc_voltage_pb0, p->adc_result_pb0);
            p->run_sub_state = RUN_SUB_PWM_OFF;
            break;
        case RUN_SUB_PWM_OFF:
            drv_pwm_off(PWM_CH_PB6);
            rtt_print_pwm(0, 0);
            p->soft_state = SOFT_RUN_STEP2;
            p->run_sub_state = RUN_SUB_PWM_ON;
            break;
        default: break;
        }
        break;

    case SOFT_RUN_STEP2:
        switch (p->run_sub_state) {
        case RUN_SUB_PWM_ON:
            drv_pwm_on(PWM_CH_PB7, PWM_DUTY_DEFAULT);
            rtt_print_pwm(1, 1);
            p->run_sub_state = RUN_SUB_WAIT_STABLE;
            p->sub_timeout = now + TIMEOUT_PWM_STABLE;
            break;
        case RUN_SUB_WAIT_STABLE:
            if (now >= p->sub_timeout) p->run_sub_state = RUN_SUB_ADC_SAMPLE;
            break;
        case RUN_SUB_ADC_SAMPLE:
            p->adc_value_pb1 = drv_adc_read(ADC_CH_PB1);
            p->adc_voltage_pb1 = drv_adc_to_voltage(p->adc_value_pb1);
            check_voltage(p, 1);
            rtt_print_adc(1, p->adc_value_pb1, p->adc_voltage_pb1, p->adc_result_pb1);
            p->run_sub_state = RUN_SUB_PWM_OFF;
            break;
        case RUN_SUB_PWM_OFF:
            drv_pwm_off(PWM_CH_PB7);
            rtt_print_pwm(1, 0);
            p->soft_state = SOFT_RUN_STEP3;
            p->run_sub_state = RUN_SUB_PWM_ON;
            break;
        default: break;
        }
        break;

    case SOFT_RUN_STEP3:
        switch (p->run_sub_state) {
        case RUN_SUB_PWM_ON:
            drv_pwm_on(PWM_CH_PB6, PWM_DUTY_DEFAULT);
            rtt_print_pwm(0, 1);
            p->run_sub_state = RUN_SUB_WAIT_STABLE;
            p->sub_timeout = now + TIMEOUT_PWM_STABLE;
            break;
        case RUN_SUB_WAIT_STABLE:
            if (now >= p->sub_timeout) p->run_sub_state = RUN_SUB_ADC_SAMPLE;
            break;
        case RUN_SUB_ADC_SAMPLE:
            p->adc_value_pb0 = drv_adc_read(ADC_CH_PB0);
            p->adc_voltage_pb0 = drv_adc_to_voltage(p->adc_value_pb0);
            check_voltage(p, 0);
            rtt_print_adc(0, p->adc_value_pb0, p->adc_voltage_pb0, p->adc_result_pb0);
            p->run_sub_state = RUN_SUB_PWM_OFF;
            break;
        case RUN_SUB_PWM_OFF:
            drv_pwm_off(PWM_CH_PB6);
            rtt_print_pwm(0, 0);
            p->soft_state = SOFT_RUN_DONE;
            p->state_timeout = 0;
            rtt_print_msg("[SYS] === RUN cycle done, 10s to IDLE ===\r\n");
            break;
        default: break;
        }
        break;

    case SOFT_RUN_DONE:
        if (p->state_timeout == 0) p->state_timeout = now + TIMEOUT_RUN_TO_IDLE;
        if (now >= p->state_timeout) {
            p->soft_state = SOFT_IDLE;
            p->state_timeout = now + TIMEOUT_IDLE_TO_RUN;
            p->run_sub_state = RUN_SUB_IDLE;
            rtt_print_msg("[SYS] Back to IDLE, 10s to next RUN\r\n");
        }
        break;

    default:
        p->soft_state = SOFT_INIT;
        break;
    }
}
