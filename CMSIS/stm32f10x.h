#ifndef __STM32F10x_H
#define __STM32F10x_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/* 类型定义 */
typedef enum { RESET = 0, SET = !RESET } FlagStatus, ITStatus;
typedef enum { DISABLE = 0, ENABLE = !DISABLE } FunctionalState;
typedef enum { ERROR = 0, SUCCESS = !ERROR } ErrorStatus;

#define __IO    volatile

/* ========== 中断号 ========== */
typedef enum IRQn {
    NonMaskableInt_IRQn   = -14,
    HardFault_IRQn        = -13,
    MemoryManagement_IRQn = -12,
    BusFault_IRQn         = -11,
    UsageFault_IRQn       = -10,
    SVCall_IRQn           = -5,
    DebugMonitor_IRQn     = -4,
    PendSV_IRQn           = -2,
    SysTick_IRQn          = -1,
    WWDG_IRQn             = 0,
    PVD_IRQn              = 1,
    TAMPER_IRQn           = 2,
    RTC_IRQn              = 3,
    FLASH_IRQn            = 4,
    RCC_IRQn              = 5,
    EXTI0_IRQn            = 6,
    EXTI1_IRQn            = 7,
    EXTI2_IRQn            = 8,
    EXTI3_IRQn            = 9,
    EXTI4_IRQn            = 10,
    DMA1_Channel1_IRQn    = 11,
    DMA1_Channel2_IRQn    = 12,
    DMA1_Channel3_IRQn    = 13,
    DMA1_Channel4_IRQn    = 14,
    DMA1_Channel5_IRQn    = 15,
    DMA1_Channel6_IRQn    = 16,
    DMA1_Channel7_IRQn    = 17,
    ADC1_2_IRQn           = 18,
    TIM1_BRK_IRQn         = 24,
    TIM1_UP_IRQn          = 25,
    TIM1_TRG_COM_IRQn     = 26,
    TIM1_CC_IRQn          = 27,
    TIM2_IRQn             = 28,
    TIM3_IRQn             = 29,
    TIM4_IRQn             = 30,
    I2C1_EV_IRQn          = 31,
    I2C1_ER_IRQn          = 32,
    USART1_IRQn           = 37,
} IRQn_Type;

#define __NVIC_PRIO_BITS  4

/* ========== 外设基地址 ========== */
#define PERIPH_BASE        ((uint32_t)0x40000000)
#define APB1PERIPH_BASE    PERIPH_BASE
#define APB2PERIPH_BASE    (PERIPH_BASE + 0x10000)
#define AHBPERIPH_BASE     (PERIPH_BASE + 0x20000)

#define GPIOA_BASE         (APB2PERIPH_BASE + 0x0800)
#define GPIOB_BASE         (APB2PERIPH_BASE + 0x0C00)
#define GPIOC_BASE         (APB2PERIPH_BASE + 0x1000)
#define GPIOD_BASE         (APB2PERIPH_BASE + 0x1400)
#define RCC_BASE           (AHBPERIPH_BASE  + 0x1000)
#define ADC1_BASE          (APB2PERIPH_BASE + 0x2400)
#define ADC2_BASE          (APB2PERIPH_BASE + 0x2800)
#define TIM1_BASE          (APB2PERIPH_BASE + 0x0000)
#define TIM2_BASE          (APB1PERIPH_BASE + 0x0000)
#define TIM3_BASE          (APB1PERIPH_BASE + 0x0400)
#define TIM4_BASE          (APB1PERIPH_BASE + 0x0800)
#define FLASH_R_BASE       (AHBPERIPH_BASE  + 0x2000)

/* ========== GPIO ========== */
typedef struct {
    __IO uint32_t CRL;
    __IO uint32_t CRH;
    __IO uint32_t IDR;
    __IO uint32_t ODR;
    __IO uint32_t BSRR;
    __IO uint32_t BRR;
    __IO uint32_t LCKR;
} GPIO_TypeDef;

#define GPIOA   ((GPIO_TypeDef *)GPIOA_BASE)
#define GPIOB   ((GPIO_TypeDef *)GPIOB_BASE)
#define GPIOC   ((GPIO_TypeDef *)GPIOC_BASE)
#define GPIOD   ((GPIO_TypeDef *)GPIOD_BASE)

#define GPIO_Pin_0     ((uint16_t)0x0001)
#define GPIO_Pin_1     ((uint16_t)0x0002)
#define GPIO_Pin_2     ((uint16_t)0x0004)
#define GPIO_Pin_3     ((uint16_t)0x0008)
#define GPIO_Pin_4     ((uint16_t)0x0010)
#define GPIO_Pin_5     ((uint16_t)0x0020)
#define GPIO_Pin_6     ((uint16_t)0x0040)
#define GPIO_Pin_7     ((uint16_t)0x0080)
#define GPIO_Pin_8     ((uint16_t)0x0100)
#define GPIO_Pin_9     ((uint16_t)0x0200)
#define GPIO_Pin_10    ((uint16_t)0x0400)
#define GPIO_Pin_11    ((uint16_t)0x0800)
#define GPIO_Pin_12    ((uint16_t)0x1000)
#define GPIO_Pin_13    ((uint16_t)0x2000)
#define GPIO_Pin_14    ((uint16_t)0x4000)
#define GPIO_Pin_15    ((uint16_t)0x8000)
#define GPIO_Pin_All   ((uint16_t)0xFFFF)

typedef enum {
    GPIO_Mode_AIN         = 0x00,
    GPIO_Mode_IN_FLOATING = 0x04,
    GPIO_Mode_IPD         = 0x28,
    GPIO_Mode_IPU         = 0x48,
    GPIO_Mode_Out_OD      = 0x14,
    GPIO_Mode_Out_PP      = 0x10,
    GPIO_Mode_AF_OD       = 0x1C,
    GPIO_Mode_AF_PP       = 0x18,
} GPIOMode_TypeDef;

typedef enum {
    GPIO_Speed_10MHz = 1,
    GPIO_Speed_2MHz  = 2,
    GPIO_Speed_50MHz = 3,
} GPIOSpeed_TypeDef;

typedef struct {
    uint16_t GPIO_Pin;
    GPIOSpeed_TypeDef GPIO_Speed;
    GPIOMode_TypeDef GPIO_Mode;
} GPIO_InitTypeDef;

typedef enum { Bit_RESET = 0, Bit_SET = !Bit_RESET } BitAction;

/* ========== RCC ========== */
typedef struct {
    __IO uint32_t CR;
    __IO uint32_t CFGR;
    __IO uint32_t CIR;
    __IO uint32_t APB2RSTR;
    __IO uint32_t APB1RSTR;
    __IO uint32_t AHBENR;
    __IO uint32_t APB2ENR;
    __IO uint32_t APB1ENR;
    __IO uint32_t BDCR;
    __IO uint32_t CSR;
} RCC_TypeDef;

#define RCC  ((RCC_TypeDef *)RCC_BASE)

#define RCC_APB2Periph_AFIO       ((uint32_t)0x00000001)
#define RCC_APB2Periph_GPIOA      ((uint32_t)0x00000004)
#define RCC_APB2Periph_GPIOB      ((uint32_t)0x00000008)
#define RCC_APB2Periph_GPIOC      ((uint32_t)0x00000010)
#define RCC_APB2Periph_GPIOD      ((uint32_t)0x00000020)
#define RCC_APB2Periph_ADC1       ((uint32_t)0x00000200)
#define RCC_APB2Periph_ADC2       ((uint32_t)0x00000400)
#define RCC_APB2Periph_TIM1       ((uint32_t)0x00000800)
#define RCC_APB2Periph_USART1     ((uint32_t)0x00004000)
#define RCC_APB1Periph_TIM2       ((uint32_t)0x00000001)
#define RCC_APB1Periph_TIM3       ((uint32_t)0x00000002)
#define RCC_APB1Periph_TIM4       ((uint32_t)0x00000004)
#define RCC_APB1Periph_USART2     ((uint32_t)0x00020000)
#define RCC_APB1Periph_USART3     ((uint32_t)0x00040000)

#define RCC_HSE_ON                ((uint32_t)0x00010000)
#define RCC_PLLSource_HSE_Div1    ((uint32_t)0x00010000)
#define RCC_PLLSource_HSE_Div2    ((uint32_t)0x00030000)
#define RCC_PLLMul_9              ((uint32_t)0x001C0000)

#define RCC_SYSCLKSource_PLLCLK   ((uint32_t)0x00000002)
#define RCC_HCLK_Div1             ((uint32_t)0x00000000)
#define RCC_HCLK_Div2             ((uint32_t)0x00000010)

#define RCC_FLAG_HSIRDY  ((uint8_t)0x21)
#define RCC_FLAG_HSERDY  ((uint8_t)0x31)
#define RCC_FLAG_PLLRDY  ((uint8_t)0x39)

#define FLASH_Latency_2                ((uint32_t)0x00000002)
#define FLASH_PrefetchBuffer_Enable    ((uint32_t)0x00000010)

/* ========== ADC ========== */
typedef struct {
    __IO uint32_t SR;
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMPR1;
    __IO uint32_t SMPR2;
    __IO uint32_t JOFR1;
    __IO uint32_t JOFR2;
    __IO uint32_t JOFR3;
    __IO uint32_t JOFR4;
    __IO uint32_t HTR;
    __IO uint32_t LTR;
    __IO uint32_t SQR1;
    __IO uint32_t SQR2;
    __IO uint32_t SQR3;
    __IO uint32_t JSQR;
    __IO uint32_t JDR1;
    __IO uint32_t JDR2;
    __IO uint32_t JDR3;
    __IO uint32_t JDR4;
    __IO uint32_t DR;
} ADC_TypeDef;

#define ADC1  ((ADC_TypeDef *)ADC1_BASE)
#define ADC2  ((ADC_TypeDef *)ADC2_BASE)

#define ADC_Channel_0   ((uint8_t)0x00)
#define ADC_Channel_1   ((uint8_t)0x01)
#define ADC_Channel_2   ((uint8_t)0x02)
#define ADC_Channel_3   ((uint8_t)0x03)
#define ADC_Channel_4   ((uint8_t)0x04)
#define ADC_Channel_5   ((uint8_t)0x05)
#define ADC_Channel_6   ((uint8_t)0x06)
#define ADC_Channel_7   ((uint8_t)0x07)
#define ADC_Channel_8   ((uint8_t)0x08)
#define ADC_Channel_9   ((uint8_t)0x09)
#define ADC_Channel_10  ((uint8_t)0x0A)
#define ADC_Channel_11  ((uint8_t)0x0B)
#define ADC_Channel_12  ((uint8_t)0x0C)
#define ADC_Channel_13  ((uint8_t)0x0D)
#define ADC_Channel_14  ((uint8_t)0x0E)
#define ADC_Channel_15  ((uint8_t)0x0F)
#define ADC_Channel_16  ((uint8_t)0x10)
#define ADC_Channel_17  ((uint8_t)0x11)

#define ADC_Mode_Independent             ((uint32_t)0x00000000)
#define ADC_ExternalTrigConv_None        ((uint32_t)0x000E0000)
#define ADC_DataAlign_Right              ((uint32_t)0x00000000)

#define ADC_SampleTime_1Cycles5    ((uint8_t)0x00)
#define ADC_SampleTime_7Cycles5    ((uint8_t)0x01)
#define ADC_SampleTime_13Cycles5   ((uint8_t)0x02)
#define ADC_SampleTime_28Cycles5   ((uint8_t)0x03)
#define ADC_SampleTime_41Cycles5   ((uint8_t)0x04)
#define ADC_SampleTime_55Cycles5   ((uint8_t)0x05)
#define ADC_SampleTime_71Cycles5   ((uint8_t)0x06)
#define ADC_SampleTime_239Cycles5  ((uint8_t)0x07)

#define ADC_FLAG_EOC  ((uint8_t)0x02)

typedef struct {
    uint32_t ADC_Mode;
    FunctionalState ADC_ScanConvMode;
    FunctionalState ADC_ContinuousConvMode;
    uint32_t ADC_ExternalTrigConv;
    uint32_t ADC_DataAlign;
    uint8_t  ADC_NbrOfChannel;
} ADC_InitTypeDef;

/* ========== TIM ========== */
typedef struct {
    __IO uint32_t CR1;
    __IO uint32_t CR2;
    __IO uint32_t SMCR;
    __IO uint32_t DIER;
    __IO uint32_t SR;
    __IO uint32_t EGR;
    __IO uint32_t CCMR1;
    __IO uint32_t CCMR2;
    __IO uint32_t CCER;
    __IO uint32_t CNT;
    __IO uint32_t PSC;
    __IO uint32_t ARR;
    __IO uint32_t RCR;
    __IO uint32_t CCR1;
    __IO uint32_t CCR2;
    __IO uint32_t CCR3;
    __IO uint32_t CCR4;
    __IO uint32_t BDTR;
    __IO uint32_t DCR;
    __IO uint32_t DMAR;
} TIM_TypeDef;

#define TIM1  ((TIM_TypeDef *)TIM1_BASE)
#define TIM2  ((TIM_TypeDef *)TIM2_BASE)
#define TIM3  ((TIM_TypeDef *)TIM3_BASE)
#define TIM4  ((TIM_TypeDef *)TIM4_BASE)

typedef struct {
    uint16_t TIM_Prescaler;
    uint16_t TIM_CounterMode;
    uint32_t TIM_Period;
    uint16_t TIM_ClockDivision;
    uint8_t  TIM_RepetitionCounter;
} TIM_TimeBaseInitTypeDef;

typedef struct {
    uint16_t TIM_OCMode;
    uint16_t TIM_OutputState;
    uint16_t TIM_OutputNState;
    uint16_t TIM_Pulse;
    uint16_t TIM_OCPolarity;
    uint16_t TIM_OCNPolarity;
    uint16_t TIM_OCIdleState;
    uint16_t TIM_OCNIdleState;
} TIM_OCInitTypeDef;

#define TIM_CounterMode_Up       ((uint16_t)0x0000)
#define TIM_CKD_DIV1             ((uint16_t)0x0000)
#define TIM_OCMode_PWM1          ((uint16_t)0x0060)
#define TIM_OutputState_Disable  ((uint16_t)0x0000)
#define TIM_OutputState_Enable   ((uint16_t)0x0001)
#define TIM_OCPolarity_High      ((uint16_t)0x0000)
#define TIM_OCPreload_Enable     ((uint16_t)0x0008)
#define TIM_Channel_1            ((uint16_t)0x0000)
#define TIM_Channel_2            ((uint16_t)0x0004)
#define TIM_CCx_Enable           ((uint16_t)0x0001)
#define TIM_CCx_Disable          ((uint16_t)0x0000)

/* ========== FLASH ========== */
typedef struct {
    __IO uint32_t ACR;
    __IO uint32_t KEYR;
    __IO uint32_t OPTKEYR;
    __IO uint32_t SR;
    __IO uint32_t CR;
    __IO uint32_t AR;
    __IO uint32_t RESERVED;
    __IO uint32_t OBR;
    __IO uint32_t WRPR;
} FLASH_TypeDef;

#define FLASH  ((FLASH_TypeDef *)FLASH_R_BASE)

/* ========== NVIC (CMSIS) ========== */
#define SCS_BASE       (0xE000E000UL)
#define NVIC_BASE      (SCS_BASE + 0x0100UL)
#define SCB_BASE       (SCS_BASE + 0x0D00UL)
#define SysTick_BASE   (SCS_BASE + 0x0010UL)

typedef struct {
    __IO uint32_t ISER[8];
    __IO uint32_t RESERVED0[24];
    __IO uint32_t ICER[8];
    __IO uint32_t RSERVED1[24];
    __IO uint32_t ISPR[8];
    __IO uint32_t RESERVED2[24];
    __IO uint32_t ICPR[8];
    __IO uint32_t RESERVED3[24];
    __IO uint32_t IABR[8];
    __IO uint32_t RESERVED4[56];
    __IO uint32_t IP[240];
} NVIC_Type;

typedef struct {
    __IO uint32_t DHCSR;
    __IO uint32_t DCRSR;
    __IO uint32_t DCRDR;
    __IO uint32_t DEMCR;
} CoreDebug_Type;

typedef struct {
    __IO uint32_t CPUID;
    __IO uint32_t ICSR;
    __IO uint32_t VTOR;
    __IO uint32_t AIRCR;
    __IO uint32_t SCR;
    __IO uint32_t CCR;
    __IO uint8_t  SHPR[12];
    __IO uint32_t SHCSR;
    __IO uint32_t CFSR;
    __IO uint32_t HFSR;
    __IO uint32_t DFSR;
    __IO uint32_t MMFAR;
    __IO uint32_t BFAR;
    __IO uint32_t AFSR;
} SCB_Type;

typedef struct {
    __IO uint32_t CTRL;
    __IO uint32_t LOAD;
    __IO uint32_t VAL;
    __IO uint32_t CALIB;
} SysTick_Type;

#define NVIC      ((NVIC_Type     *)NVIC_BASE)
#define SCB       ((SCB_Type      *)SCB_BASE)
#define SysTick   ((SysTick_Type  *)SysTick_BASE)

/* 系统时钟 */
extern uint32_t SystemCoreClock;

#ifdef __cplusplus
}
#endif

#ifdef USE_STDPERIPH_DRIVER
  #include "stm32f10x_conf.h"
#endif

#endif /* __STM32F10x_H */
