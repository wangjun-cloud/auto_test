        MODULE  ?cstartup

        SECTION CSTACK:DATA:NOROOT(3)
        SECTION .intvec:CODE:ROOT(2)

        EXTERN  __iar_program_start
        PUBLIC  __vector_table

__vector_table
        DCD     sfe(CSTACK)
        DCD     Reset_Handler
        DCD     NMI_Handler
        DCD     HardFault_Handler
        DCD     MemManage_Handler
        DCD     BusFault_Handler
        DCD     UsageFault_Handler
        DCD     0, 0, 0, 0
        DCD     SVC_Handler
        DCD     DebugMon_Handler
        DCD     0
        DCD     PendSV_Handler
        DCD     SysTick_Handler
        DCD     WWDG_IRQHandler
        DCD     PVD_IRQHandler
        DCD     TAMPER_IRQHandler
        DCD     RTC_IRQHandler
        DCD     FLASH_IRQHandler
        DCD     RCC_IRQHandler
        DCD     EXTI0_IRQHandler
        DCD     EXTI1_IRQHandler
        DCD     EXTI2_IRQHandler
        DCD     EXTI3_IRQHandler
        DCD     EXTI4_IRQHandler
        DCD     DMA1_Channel1_IRQHandler
        DCD     DMA1_Channel2_IRQHandler
        DCD     DMA1_Channel3_IRQHandler
        DCD     DMA1_Channel4_IRQHandler
        DCD     DMA1_Channel5_IRQHandler
        DCD     DMA1_Channel6_IRQHandler
        DCD     DMA1_Channel7_IRQHandler
        DCD     ADC1_2_IRQHandler
        DCD     USB_HP_CAN1_TX_IRQHandler
        DCD     USB_LP_CAN1_RX0_IRQHandler
        DCD     CAN1_RX1_IRQHandler
        DCD     CAN1_SCE_IRQHandler
        DCD     EXTI9_5_IRQHandler
        DCD     TIM1_BRK_IRQHandler
        DCD     TIM1_UP_IRQHandler
        DCD     TIM1_TRG_COM_IRQHandler
        DCD     TIM1_CC_IRQHandler
        DCD     TIM2_IRQHandler
        DCD     TIM3_IRQHandler
        DCD     TIM4_IRQHandler
        DCD     I2C1_EV_IRQHandler
        DCD     I2C1_ER_IRQHandler
        DCD     I2C2_EV_IRQHandler
        DCD     I2C2_ER_IRQHandler
        DCD     SPI1_IRQHandler
        DCD     SPI2_IRQHandler
        DCD     USART1_IRQHandler
        DCD     USART2_IRQHandler
        DCD     USART3_IRQHandler
        DCD     EXTI15_10_IRQHandler
        DCD     RTCAlarm_IRQHandler
        DCD     USBWakeUp_IRQHandler

        THUMB

        PUBWEAK Reset_Handler
        SECTION .text:CODE:REORDER:NOROOT(2)
Reset_Handler
        LDR     R0, =__iar_program_start
        BX      R0

        PUBWEAK NMI_Handler
        PUBWEAK HardFault_Handler
        PUBWEAK MemManage_Handler
        PUBWEAK BusFault_Handler
        PUBWEAK UsageFault_Handler
        PUBWEAK SVC_Handler
        PUBWEAK DebugMon_Handler
        PUBWEAK PendSV_Handler
        PUBWEAK SysTick_Handler
        PUBWEAK WWDG_IRQHandler
        PUBWEAK PVD_IRQHandler
        PUBWEAK TAMPER_IRQHandler
        PUBWEAK RTC_IRQHandler
        PUBWEAK FLASH_IRQHandler
        PUBWEAK RCC_IRQHandler
        PUBWEAK EXTI0_IRQHandler
        PUBWEAK EXTI1_IRQHandler
        PUBWEAK EXTI2_IRQHandler
        PUBWEAK EXTI3_IRQHandler
        PUBWEAK EXTI4_IRQHandler
        PUBWEAK DMA1_Channel1_IRQHandler
        PUBWEAK DMA1_Channel2_IRQHandler
        PUBWEAK DMA1_Channel3_IRQHandler
        PUBWEAK DMA1_Channel4_IRQHandler
        PUBWEAK DMA1_Channel5_IRQHandler
        PUBWEAK DMA1_Channel6_IRQHandler
        PUBWEAK DMA1_Channel7_IRQHandler
        PUBWEAK ADC1_2_IRQHandler
        PUBWEAK USB_HP_CAN1_TX_IRQHandler
        PUBWEAK USB_LP_CAN1_RX0_IRQHandler
        PUBWEAK CAN1_RX1_IRQHandler
        PUBWEAK CAN1_SCE_IRQHandler
        PUBWEAK EXTI9_5_IRQHandler
        PUBWEAK TIM1_BRK_IRQHandler
        PUBWEAK TIM1_UP_IRQHandler
        PUBWEAK TIM1_TRG_COM_IRQHandler
        PUBWEAK TIM1_CC_IRQHandler
        PUBWEAK TIM2_IRQHandler
        PUBWEAK TIM3_IRQHandler
        PUBWEAK TIM4_IRQHandler
        PUBWEAK I2C1_EV_IRQHandler
        PUBWEAK I2C1_ER_IRQHandler
        PUBWEAK I2C2_EV_IRQHandler
        PUBWEAK I2C2_ER_IRQHandler
        PUBWEAK SPI1_IRQHandler
        PUBWEAK SPI2_IRQHandler
        PUBWEAK USART1_IRQHandler
        PUBWEAK USART2_IRQHandler
        PUBWEAK USART3_IRQHandler
        PUBWEAK EXTI15_10_IRQHandler
        PUBWEAK RTCAlarm_IRQHandler
        PUBWEAK USBWakeUp_IRQHandler

        SECTION .text:CODE:REORDER:NOROOT(1)
NMI_Handler
HardFault_Handler
MemManage_Handler
BusFault_Handler
UsageFault_Handler
SVC_Handler
DebugMon_Handler
PendSV_Handler
SysTick_Handler
WWDG_IRQHandler
PVD_IRQHandler
TAMPER_IRQHandler
RTC_IRQHandler
FLASH_IRQHandler
RCC_IRQHandler
EXTI0_IRQHandler
EXTI1_IRQHandler
EXTI2_IRQHandler
EXTI3_IRQHandler
EXTI4_IRQHandler
DMA1_Channel1_IRQHandler
DMA1_Channel2_IRQHandler
DMA1_Channel3_IRQHandler
DMA1_Channel4_IRQHandler
DMA1_Channel5_IRQHandler
DMA1_Channel6_IRQHandler
DMA1_Channel7_IRQHandler
ADC1_2_IRQHandler
USB_HP_CAN1_TX_IRQHandler
USB_LP_CAN1_RX0_IRQHandler
CAN1_RX1_IRQHandler
CAN1_SCE_IRQHandler
EXTI9_5_IRQHandler
TIM1_BRK_IRQHandler
TIM1_UP_IRQHandler
TIM1_TRG_COM_IRQHandler
TIM1_CC_IRQHandler
TIM2_IRQHandler
TIM3_IRQHandler
TIM4_IRQHandler
I2C1_EV_IRQHandler
I2C1_ER_IRQHandler
I2C2_EV_IRQHandler
I2C2_ER_IRQHandler
SPI1_IRQHandler
SPI2_IRQHandler
USART1_IRQHandler
USART2_IRQHandler
USART3_IRQHandler
EXTI15_10_IRQHandler
RTCAlarm_IRQHandler
USBWakeUp_IRQHandler
Default_Handler
        B       Default_Handler

        END
