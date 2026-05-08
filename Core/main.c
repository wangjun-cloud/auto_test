#include "stm32f10x.h"
#include "mydev.h"
#include "mainprocess.h"
#include "my_rtt_debug.h"

static volatile uint32_t g_sys_tick = 0;

uint32_t get_local_absolute_time(void) { return g_sys_tick; }

void SysTick_Handler(void) { g_sys_tick++; }

static void system_clock_init(void)
{
    RCC_DeInit();
    RCC_HSEConfig(RCC_HSE_ON);
    while (RCC_WaitForHSEStartUp() != SUCCESS);

    FLASH_SetLatency(FLASH_Latency_2);
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

    RCC_HCLKConfig(RCC_HCLK_Div1);
    RCC_PCLK1Config(RCC_HCLK_Div2);
    RCC_PCLK2Config(RCC_HCLK_Div1);
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_9);

    RCC_PLLCmd(ENABLE);
    while (!RCC_GetFlagStatus(RCC_FLAG_PLLRDY));

    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
    while (RCC_GetSYSCLKSource() != 0x08);
}

int main(void)
{
    system_clock_init();
    SysTick->LOAD = 72000 - 1;
    SysTick->VAL  = 0;
    SysTick->CTRL = 0x07;

    rtt_debug_init();
    rtt_print_msg("[SYS] === AUTO TEST START ===\r\n");

    mydev_init(&TMyDev);

    while (1) {
        mainprocess_parse(&TMyDev);
    }
}
