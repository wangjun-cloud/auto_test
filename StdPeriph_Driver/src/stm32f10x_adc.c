#include "stm32f10x_adc.h"

void ADC_DeInit(ADC_TypeDef *ADCx)
{
    if (ADCx == ADC1) {
        RCC->APB2RSTR |= RCC_APB2Periph_ADC1;
        RCC->APB2RSTR &= ~RCC_APB2Periph_ADC1;
    }
}

void ADC_Init(ADC_TypeDef *ADCx, ADC_InitTypeDef *s)
{
    uint32_t tmp;

    tmp = ADCx->CR1 & ~0x003F0000;
    tmp |= (uint32_t)(s->ADC_Mode << 16);
    if (s->ADC_ScanConvMode) tmp |= (1 << 8);
    ADCx->CR1 = tmp;

    tmp = ADCx->CR2 & ~0x401E0000;
    tmp |= s->ADC_ExternalTrigConv | s->ADC_DataAlign;
    if (s->ADC_ContinuousConvMode) tmp |= (1 << 1);
    ADCx->CR2 = tmp;

    tmp = ADCx->SQR1 & ~0x00F00000;
    tmp |= (uint32_t)((s->ADC_NbrOfChannel - 1) << 20);
    ADCx->SQR1 = tmp;
}

void ADC_Cmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if (NewState) ADCx->CR2 |= 1;
    else ADCx->CR2 &= ~1;
}

void ADC_ResetCalibration(ADC_TypeDef *ADCx) { ADCx->CR2 |= (1 << 3); }
FlagStatus ADC_GetResetCalibrationStatus(ADC_TypeDef *ADCx) {
    return (ADCx->CR2 & (1 << 3)) ? SET : RESET;
}
void ADC_StartCalibration(ADC_TypeDef *ADCx) { ADCx->CR2 |= (1 << 2); }
FlagStatus ADC_GetCalibrationStatus(ADC_TypeDef *ADCx) {
    return (ADCx->CR2 & (1 << 2)) ? SET : RESET;
}

void ADC_RegularChannelConfig(ADC_TypeDef *ADCx, uint8_t ch, uint8_t rank, uint8_t st)
{
    uint32_t tmp;
    if (rank < 7) {
        tmp = ADCx->SQR3 & ~(0x1F << (5 * (rank - 1)));
        ADCx->SQR3 = tmp | ((uint32_t)ch << (5 * (rank - 1)));
    }
    if (ch < 10) {
        tmp = ADCx->SMPR2 & ~(0x07 << (3 * ch));
        ADCx->SMPR2 = tmp | ((uint32_t)st << (3 * ch));
    } else {
        tmp = ADCx->SMPR1 & ~(0x07 << (3 * (ch - 10)));
        ADCx->SMPR1 = tmp | ((uint32_t)st << (3 * (ch - 10)));
    }
}

void ADC_SoftwareStartConvCmd(ADC_TypeDef *ADCx, FunctionalState NewState)
{
    if (NewState) ADCx->CR2 |= (1 << 22);
    else ADCx->CR2 &= ~(1 << 22);
}

FlagStatus ADC_GetFlagStatus(ADC_TypeDef *ADCx, uint8_t flag) {
    return (ADCx->SR & flag) ? SET : RESET;
}

uint16_t ADC_GetConversionValue(ADC_TypeDef *ADCx) {
    return (uint16_t)(ADCx->DR);
}
