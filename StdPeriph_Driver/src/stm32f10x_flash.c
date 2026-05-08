#include "stm32f10x_flash.h"

void FLASH_SetLatency(uint32_t lat)
{
    uint32_t tmp = FLASH->ACR & ~0x03;
    FLASH->ACR = tmp | lat;
}

void FLASH_PrefetchBufferCmd(uint32_t cmd)
{
    FLASH->ACR |= cmd;
}
