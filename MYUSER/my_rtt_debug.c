#include "my_rtt_debug.h"

#if DEBUG_PRINT_ENABLE

#include "SEGGER_RTT.h"
#include <string.h>
#include <stdio.h>

void rtt_debug_init(void) { SEGGER_RTT_Init(); }

static const char *state_name(uint8_t s)
{
    switch (s) {
    case 0: return "INIT";
    case 1: return "IDLE";
    case 2: return "RUN";
    case 3: return "STEP1(PB6+ADC)";
    case 4: return "STEP2(PB7+ADC)";
    case 5: return "STEP3(PB6+ADC)";
    case 6: return "DONE";
    default: return "?";
    }
}

static const char *result_name(uint8_t r)
{
    switch (r) {
    case 0: return "NONE";
    case 1: return "OK(0~3.3V)";
    case 2: return "ABNORMAL!";
    default: return "?";
    }
}

void rtt_print_state(uint8_t state, uint8_t sub)
{
    char buf[128];
    int n = snprintf(buf, sizeof(buf), "[STATE] %s (sub=%d)\r\n", state_name(state), sub);
    SEGGER_RTT_Write(0, buf, n);
}

void rtt_print_adc(uint8_t ch, uint16_t raw, uint16_t vol, uint8_t res)
{
    char buf[128];
    int n = snprintf(buf, sizeof(buf), "[ADC] PB%d: raw=%d, %dmV, %s\r\n",
                     ch, raw, vol, result_name(res));
    SEGGER_RTT_Write(0, buf, n);
}

void rtt_print_pwm(uint8_t ch, uint8_t on_off)
{
    char buf[64];
    int n = snprintf(buf, sizeof(buf), "[PWM] PB%d: %s\r\n", ch ? 7 : 6, on_off ? "ON" : "OFF");
    SEGGER_RTT_Write(0, buf, n);
}

void rtt_print_msg(const char *msg)
{
    SEGGER_RTT_Write(0, msg, strlen(msg));
}

#endif
