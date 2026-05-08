#ifndef __STM32F10x_FLASH_H
#define __STM32F10x_FLASH_H

#include "stm32f10x.h"

void FLASH_SetLatency(uint32_t FLASH_Latency);
void FLASH_PrefetchBufferCmd(uint32_t FLASH_PrefetchBuffer);

#endif
