#ifndef __MY_RTT_DEBUG_H
#define __MY_RTT_DEBUG_H

#include "stm32f10x.h"

#define DEBUG_PRINT_ENABLE  1

#if DEBUG_PRINT_ENABLE
void rtt_debug_init(void);
void rtt_print_state(uint8_t state, uint8_t sub_state);
void rtt_print_adc(uint8_t ch, uint16_t raw, uint16_t vol, uint8_t res);
void rtt_print_pwm(uint8_t ch, uint8_t on_off);
void rtt_print_msg(const char *msg);
#else
#define rtt_debug_init()
#define rtt_print_state(a,b)
#define rtt_print_adc(a,b,c,d)
#define rtt_print_pwm(a,b)
#define rtt_print_msg(a)
#endif

#endif
