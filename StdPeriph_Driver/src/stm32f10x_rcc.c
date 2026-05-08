#include "stm32f10x_rcc.h"

void RCC_DeInit(void)
{
    RCC->CR |= 0x00000001;
    RCC->CFGR &= 0xF8FF0000;
    RCC->CR &= 0xFEF6FFFF;
    RCC->CFGR &= 0xFF80FFFF;
    RCC->CIR = 0x009F0000;
}

void RCC_HSEConfig(uint32_t RCC_HSE)
{
    (void)RCC_HSE;
    RCC->CR &= ~(1 << 18);
    RCC->CR |= (1 << 16);
}

ErrorStatus RCC_WaitForHSEStartUp(void)
{
    volatile uint32_t timeout = 0x50000;
    while (timeout-- > 0) {
        if (RCC->CR & (1 << 17)) return SUCCESS;
    }
    return ERROR;
}

void RCC_HCLKConfig(uint32_t RCC_HCLK)
{
    uint32_t tmp = RCC->CFGR & ~0x000000F0;
    RCC->CFGR = tmp | RCC_HCLK;
}

void RCC_PCLK1Config(uint32_t RCC_HCLK)
{
    uint32_t tmp = RCC->CFGR & ~0x00000700;
    RCC->CFGR = tmp | RCC_HCLK;
}

void RCC_PCLK2Config(uint32_t RCC_HCLK)
{
    uint32_t tmp = RCC->CFGR & ~0x00003800;
    RCC->CFGR = tmp | RCC_HCLK;
}

void RCC_PLLConfig(uint32_t RCC_PLLSource, uint32_t RCC_PLLMul)
{
    uint32_t tmp = RCC->CFGR & ~0x003F0000;
    RCC->CFGR = tmp | (RCC_PLLSource | RCC_PLLMul);
}

void RCC_PLLCmd(FunctionalState NewState)
{
    if (NewState) RCC->CR |= (1 << 24);
    else RCC->CR &= ~(1 << 24);
}

void RCC_SYSCLKConfig(uint32_t RCC_SYSCLKSource)
{
    uint32_t tmp = RCC->CFGR & ~0x00000003;
    RCC->CFGR = tmp | RCC_SYSCLKSource;
}

uint8_t RCC_GetSYSCLKSource(void)
{
    return (uint8_t)(RCC->CFGR & 0x0000000C);
}

void RCC_APB2PeriphClockCmd(uint32_t RCC_APB2Periph, FunctionalState NewState)
{
    if (NewState) RCC->APB2ENR |= RCC_APB2Periph;
    else RCC->APB2ENR &= ~RCC_APB2Periph;
}

void RCC_APB1PeriphClockCmd(uint32_t RCC_APB1Periph, FunctionalState NewState)
{
    if (NewState) RCC->APB1ENR |= RCC_APB1Periph;
    else RCC->APB1ENR &= ~RCC_APB1Periph;
}

FlagStatus RCC_GetFlagStatus(uint8_t RCC_FLAG)
{
    uint32_t tmp, statusreg;
    FlagStatus bitstatus = RESET;

    if ((RCC_FLAG & 0x20) != 0) statusreg = RCC->CR;
    else if ((RCC_FLAG & 0x40) != 0) statusreg = RCC->BDCR;
    else statusreg = RCC->CSR;

    tmp = 1 << (RCC_FLAG & 0x1F);
    if (statusreg & tmp) bitstatus = SET;
    return bitstatus;
}
