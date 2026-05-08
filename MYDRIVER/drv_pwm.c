#include "drv_pwm.h"

#define PWM_PRESCALER  71
#define PWM_PERIOD     999

void drv_pwm_init(void)
{
    GPIO_InitTypeDef        gpio;
    TIM_TimeBaseInitTypeDef base;
    TIM_OCInitTypeDef       oc;

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);

    gpio.GPIO_Pin   = GPIO_Pin_6 | GPIO_Pin_7;
    gpio.GPIO_Mode  = GPIO_Mode_AF_PP;
    gpio.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &gpio);

    base.TIM_Prescaler   = PWM_PRESCALER;
    base.TIM_Period      = PWM_PERIOD;
    base.TIM_ClockDivision = TIM_CKD_DIV1;
    base.TIM_CounterMode = TIM_CounterMode_Up;
    base.TIM_RepetitionCounter = 0;
    TIM_TimeBaseInit(TIM4, &base);

    oc.TIM_OCMode      = TIM_OCMode_PWM1;
    oc.TIM_OutputState = TIM_OutputState_Disable;
    oc.TIM_Pulse       = 0;
    oc.TIM_OCPolarity  = TIM_OCPolarity_High;
    oc.TIM_OutputNState = 0;
    oc.TIM_OCNPolarity  = 0;
    oc.TIM_OCIdleState  = 0;
    oc.TIM_OCNIdleState = 0;
    TIM_OC1Init(TIM4, &oc);
    TIM_OC1PreloadConfig(TIM4, TIM_OCPreload_Enable);

    oc.TIM_OutputState = TIM_OutputState_Disable;
    oc.TIM_Pulse       = 0;
    TIM_OC2Init(TIM4, &oc);
    TIM_OC2PreloadConfig(TIM4, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM4, ENABLE);
    TIM_Cmd(TIM4, ENABLE);
}

void drv_pwm_on(uint8_t channel, uint8_t duty)
{
    uint16_t pulse;
    if (duty > 100) duty = 100;
    pulse = (uint16_t)((uint32_t)duty * PWM_PERIOD / 100);

    if (channel == PWM_CH_PB6) {
        TIM4->CCR1 = pulse;
        TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Enable);
    } else {
        TIM4->CCR2 = pulse;
        TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Enable);
    }
}

void drv_pwm_off(uint8_t channel)
{
    if (channel == PWM_CH_PB6) {
        TIM4->CCR1 = 0;
        TIM_CCxCmd(TIM4, TIM_Channel_1, TIM_CCx_Disable);
    } else {
        TIM4->CCR2 = 0;
        TIM_CCxCmd(TIM4, TIM_Channel_2, TIM_CCx_Disable);
    }
}
