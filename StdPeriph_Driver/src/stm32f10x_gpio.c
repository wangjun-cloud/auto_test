#include "stm32f10x_gpio.h"

void GPIO_Init(GPIO_TypeDef *GPIOx, GPIO_InitTypeDef *GPIO_InitStruct)
{
    uint32_t currentmode, pinpos, pos, currentpin;
    uint32_t tmpreg;

    currentmode = ((uint32_t)(GPIO_InitStruct->GPIO_Mode) & 0x0F);
    if ((((uint32_t)(GPIO_InitStruct->GPIO_Mode) & 0x10) != 0) && currentmode)
        currentmode |= (uint32_t)GPIO_InitStruct->GPIO_Speed;

    if ((GPIO_InitStruct->GPIO_Pin & 0xFF) != 0) {
        tmpreg = GPIOx->CRL;
        for (pinpos = 0; pinpos < 8; pinpos++) {
            pos = 1 << pinpos;
            currentpin = GPIO_InitStruct->GPIO_Pin & pos;
            if (currentpin == pos) {
                pos = pinpos << 2;
                tmpreg &= ~((uint32_t)0x0F << pos);
                tmpreg |= (currentmode << pos);
            }
        }
        GPIOx->CRL = tmpreg;
    }

    if (GPIO_InitStruct->GPIO_Pin > 0xFF) {
        tmpreg = GPIOx->CRH;
        for (pinpos = 0; pinpos < 8; pinpos++) {
            pos = 1 << (pinpos + 8);
            currentpin = GPIO_InitStruct->GPIO_Pin & pos;
            if (currentpin == pos) {
                pos = pinpos << 2;
                tmpreg &= ~((uint32_t)0x0F << pos);
                tmpreg |= (currentmode << pos);
            }
        }
        GPIOx->CRH = tmpreg;
    }
}

void GPIO_SetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->BSRR = GPIO_Pin;
}

void GPIO_ResetBits(GPIO_TypeDef *GPIOx, uint16_t GPIO_Pin)
{
    GPIOx->BRR = GPIO_Pin;
}
