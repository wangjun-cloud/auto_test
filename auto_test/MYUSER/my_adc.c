#include "my_dev.h"
#include "stm32f1xx_ll_adc.h"
#include "stm32f1xx_ll_gpio.h"
#include "stm32f1xx_ll_bus.h"

void my_adc_init(void)
{
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOB);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_ADC1);

    LL_GPIO_InitTypeDef gpio = {0};
    gpio.Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1;
    gpio.Mode = LL_GPIO_MODE_ANALOG;
    LL_GPIO_Init(GPIOB, &gpio);

    LL_ADC_InitTypeDef adc = {0};
    adc.DataAlignment = LL_ADC_DATA_ALIGN_RIGHT;
    adc.SequencersScanMode = LL_ADC_SEQ_SCAN_DISABLE;
    LL_ADC_Init(ADC1, &adc);

    LL_ADC_REG_InitTypeDef reg = {0};
    reg.TriggerSource = LL_ADC_REG_TRIG_SOFTWARE;
    reg.SequencerLength = LL_ADC_REG_SEQ_SCAN_DISABLE;
    reg.SequencerDiscont = LL_ADC_REG_SEQ_DISCONT_DISABLE;
    reg.ContinuousMode = LL_ADC_REG_CONV_SINGLE;
    reg.DMATransfer = LL_ADC_REG_DMA_TRANSFER_NONE;
    LL_ADC_REG_Init(ADC1, &reg);

    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_8, LL_ADC_SAMPLINGTIME_55CYCLES_5);
    LL_ADC_SetChannelSamplingTime(ADC1, LL_ADC_CHANNEL_9, LL_ADC_SAMPLINGTIME_55CYCLES_5);

    LL_ADC_Enable(ADC1);
    LL_ADC_StartCalibration(ADC1);
    while (LL_ADC_IsCalibrationOnGoing(ADC1)) {}
}

static uint16_t adc_read_channel(uint32_t channel)
{
    LL_ADC_REG_SetSequencerRanks(ADC1, LL_ADC_REG_RANK_1, channel);
    LL_ADC_REG_StartConversionSWStart(ADC1);
    while (!LL_ADC_IsActiveFlag_EOS(ADC1)) {}
    LL_ADC_ClearFlag_EOS(ADC1);
    return LL_ADC_REG_ReadConversionData12(ADC1);
}

uint16_t my_adc_read_pb0(void) { return adc_read_channel(LL_ADC_CHANNEL_8); }
uint16_t my_adc_read_pb1(void) { return adc_read_channel(LL_ADC_CHANNEL_9); }
