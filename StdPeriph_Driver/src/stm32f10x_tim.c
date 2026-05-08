#include "stm32f10x_tim.h"

void TIM_TimeBaseInit(TIM_TypeDef *TIMx, TIM_TimeBaseInitTypeDef *s)
{
    uint32_t tmp = TIMx->CR1;
    tmp &= ~(0x0060 | 0x0300);
    tmp |= (uint32_t)s->TIM_CounterMode | (uint32_t)s->TIM_ClockDivision;
    TIMx->CR1 = tmp;
    TIMx->ARR = s->TIM_Period;
    TIMx->PSC = s->TIM_Prescaler;
}

static void oc_init(TIM_TypeDef *TIMx, int ch, TIM_OCInitTypeDef *s)
{
    uint32_t ccmr, ccer;

    ccer = TIMx->CCER;
    if (ch == 0) {
        ccmr = TIMx->CCMR1 & ~(0x0073);
        ccmr |= (uint32_t)s->TIM_OCMode;
        TIMx->CCMR1 = ccmr;
        TIMx->CCR1 = s->TIM_Pulse;
        ccer &= ~(1 << 0);
        ccer |= (uint32_t)s->TIM_OutputState;
    } else {
        ccmr = TIMx->CCMR1 & ~(0x0073 << 8);
        ccmr |= ((uint32_t)s->TIM_OCMode << 8);
        TIMx->CCMR1 = ccmr;
        TIMx->CCR2 = s->TIM_Pulse;
        ccer &= ~(1 << 4);
        ccer |= ((uint32_t)s->TIM_OutputState << 4);
    }
    ccer |= (uint32_t)s->TIM_OCPolarity << (ch * 4);
    TIMx->CCER = ccer;
}

void TIM_OC1Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *s) { oc_init(TIMx, 0, s); }
void TIM_OC2Init(TIM_TypeDef *TIMx, TIM_OCInitTypeDef *s) { oc_init(TIMx, 1, s); }

void TIM_OC1PreloadConfig(TIM_TypeDef *TIMx, uint16_t v)
{
    uint32_t tmp = TIMx->CCMR1 & ~(0x0008);
    TIMx->CCMR1 = tmp | v;
}

void TIM_OC2PreloadConfig(TIM_TypeDef *TIMx, uint16_t v)
{
    uint32_t tmp = TIMx->CCMR1 & ~(0x0800);
    TIMx->CCMR1 = tmp | ((uint32_t)v << 8);
}

void TIM_ARRPreloadConfig(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    if (NewState) TIMx->CR1 |= (1 << 7);
    else TIMx->CR1 &= ~(1 << 7);
}

void TIM_Cmd(TIM_TypeDef *TIMx, FunctionalState NewState)
{
    if (NewState) TIMx->CR1 |= 1;
    else TIMx->CR1 &= ~1;
}

void TIM_CCxCmd(TIM_TypeDef *TIMx, uint16_t TIM_Channel, uint16_t TIM_CCx)
{
    uint32_t shift = 0;
    if (TIM_Channel == 0x04) shift = 4;
    uint32_t tmp = TIMx->CCER & ~(1 << shift);
    TIMx->CCER = tmp | ((uint32_t)TIM_CCx << shift);
}
