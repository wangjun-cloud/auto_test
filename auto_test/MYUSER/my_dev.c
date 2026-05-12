#include "my_dev.h"
#include "SEGGER_RTT.h"

tMyDev TMyDev;

static volatile uint32_t s_tick_ms = 0;

uint32_t get_local_absolute_time(void)
{
    return s_tick_ms;
}

void my_dev_systick_inc(void)
{
    s_tick_ms++;
}

void my_dev_init(void)
{
    TMyDev.soft_state = STATE_INIT;
    TMyDev.run_sub = RUN_IDLE_WAIT;
    TMyDev.state_tick = 0;

    SEGGER_RTT_Init();
    SEGGER_RTT_printf(0, "[INIT] Device initialized\r\n");

    my_pwm_init();
    my_adc_init();
}

static void enter_state(eSoftState s)
{
    TMyDev.soft_state = s;
    TMyDev.state_tick = s_tick_ms;

    switch (s) {
        case STATE_IDLE:
            SEGGER_RTT_printf(0, "[STATE] Enter IDLE (10s)\r\n");
            my_pwm_stop_all();
            break;
        case STATE_RUN:
            SEGGER_RTT_printf(0, "[STATE] Enter RUN\r\n");
            TMyDev.run_sub = RUN_IDLE_WAIT;
            TMyDev.state_tick = s_tick_ms;
            break;
        default:
            break;
    }
}

static void process_run(void)
{
    uint32_t elapsed = s_tick_ms - TMyDev.state_tick;

    switch (TMyDev.run_sub) {
        case RUN_IDLE_WAIT:
            if (elapsed >= 10) {
                TMyDev.run_sub = RUN_PWM_PB6_ADC;
                TMyDev.state_tick = s_tick_ms;
                my_pwm_start_pb6();
                SEGGER_RTT_printf(0, "[RUN] PB6 PWM ON, wait 50ms\r\n");
            }
            break;

        case RUN_PWM_PB6_ADC:
            if (elapsed >= 50) {
                TMyDev.adc_val[0] = my_adc_read_pb0();
                uint32_t mv = (uint32_t)TMyDev.adc_val[0] * 3300 / 4095;
                TMyDev.adc_ok[0] = (mv <= 3300) ? 1 : 0;
                SEGGER_RTT_printf(0, "[RUN] PB6 ADC=%u (%umV) %s\r\n",
                    TMyDev.adc_val[0], mv, TMyDev.adc_ok[0] ? "OK" : "FAIL");
                TMyDev.run_sub = RUN_SWITCH_TO_PB7;
                TMyDev.state_tick = s_tick_ms;
            }
            break;

        case RUN_SWITCH_TO_PB7:
            my_pwm_stop_pb6();
            my_pwm_start_pb7();
            TMyDev.run_sub = RUN_PWM_PB7_ADC;
            TMyDev.state_tick = s_tick_ms;
            SEGGER_RTT_printf(0, "[RUN] PB7 PWM ON, wait 50ms\r\n");
            break;

        case RUN_PWM_PB7_ADC:
            if (elapsed >= 50) {
                TMyDev.adc_val[1] = my_adc_read_pb1();
                uint32_t mv = (uint32_t)TMyDev.adc_val[1] * 3300 / 4095;
                TMyDev.adc_ok[1] = (mv <= 3300) ? 1 : 0;
                SEGGER_RTT_printf(0, "[RUN] PB7 ADC=%u (%umV) %s\r\n",
                    TMyDev.adc_val[1], mv, TMyDev.adc_ok[1] ? "OK" : "FAIL");
                TMyDev.run_sub = RUN_SWITCH_TO_PB6;
                TMyDev.state_tick = s_tick_ms;
            }
            break;

        case RUN_SWITCH_TO_PB6:
            my_pwm_stop_pb7();
            my_pwm_start_pb6();
            TMyDev.run_sub = RUN_PWM_PB6_ADC2;
            TMyDev.state_tick = s_tick_ms;
            SEGGER_RTT_printf(0, "[RUN] PB6 PWM ON again, wait 50ms\r\n");
            break;

        case RUN_PWM_PB6_ADC2:
            if (elapsed >= 50) {
                uint16_t val = my_adc_read_pb0();
                uint32_t mv = (uint32_t)val * 3300 / 4095;
                uint8_t ok = (mv <= 3300) ? 1 : 0;
                SEGGER_RTT_printf(0, "[RUN] PB6 ADC2=%u (%umV) %s\r\n",
                    val, mv, ok ? "OK" : "FAIL");
                my_pwm_stop_pb6();
                TMyDev.run_sub = RUN_DONE_WAIT;
                TMyDev.state_tick = s_tick_ms;
                SEGGER_RTT_printf(0, "[RUN] All tests done, wait 10s\r\n");
            }
            break;

        case RUN_DONE_WAIT:
            if (elapsed >= 10000) {
                enter_state(STATE_IDLE);
            }
            break;
    }
}

void my_dev_process(void)
{
    switch (TMyDev.soft_state) {
        case STATE_INIT:
            enter_state(STATE_IDLE);
            break;

        case STATE_IDLE:
            if ((s_tick_ms - TMyDev.state_tick) >= 10000) {
                enter_state(STATE_RUN);
            }
            break;

        case STATE_RUN:
            process_run();
            break;
    }
}
