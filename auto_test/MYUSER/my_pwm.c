#include "my_dev.h"
#include "stm32f1xx_ll_tim.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"

#define PWM_PRESCALER  (72 - 1)
#define PWM_ARR        (1000 - 1)

void my_pwm_init(void)
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);

    LL_GPIO_InitTypeDef gpio = {0};
    gpio.Pin = LL_GPIO_PIN_6 | LL_GPIO_PIN_7;
    gpio.Mode = LL_GPIO_MODE_ALTERNATE;
    gpio.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    gpio.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    LL_GPIO_Init(GPIOB, &gpio);

    LL_TIM_InitTypeDef tim = {0};
    tim.Prescaler = PWM_PRESCALER;
    tim.Autoreload = PWM_ARR;
    tim.CounterMode = LL_TIM_COUNTERMODE_UP;
    tim.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
    LL_TIM_Init(TIM4, &tim);

    LL_TIM_OC_InitTypeDef oc = {0};
    oc.OCMode = LL_TIM_OCMODE_PWM1;
    oc.OCState = LL_TIM_OCSTATE_DISABLE;
    oc.CompareValue = 500;
    oc.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
    LL_TIM_OC_Init(TIM4, LL_TIM_CHANNEL_CH1, &oc);
    LL_TIM_OC_DisablePreload(TIM4, LL_TIM_CHANNEL_CH1);

    LL_TIM_OC_Init(TIM4, LL_TIM_CHANNEL_CH2, &oc);
    LL_TIM_OC_DisablePreload(TIM4, LL_TIM_CHANNEL_CH2);

    LL_TIM_CC_DisableChannel(TIM4, LL_TIM_CHANNEL_CH1);
    LL_TIM_CC_DisableChannel(TIM4, LL_TIM_CHANNEL_CH2);

    LL_TIM_EnableCounter(TIM4);
}

void my_pwm_start_pb6(void)  { LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1); }
void my_pwm_stop_pb6(void)   { LL_TIM_CC_DisableChannel(TIM4, LL_TIM_CHANNEL_CH1); }
void my_pwm_start_pb7(void)  { LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH2); }
void my_pwm_stop_pb7(void)   { LL_TIM_CC_DisableChannel(TIM4, LL_TIM_CHANNEL_CH2); }
void my_pwm_stop_all(void)   { my_pwm_stop_pb6(); my_pwm_stop_pb7(); }
