# STM32F103 64脚（LQFP64）GPIO 功能参考文档

> 适用型号：STM32F103R8T6 / STM32F103RBT6（64脚，Flash 64KB/128KB）
>
> 说明：STM32F103C8T6 标称48脚封装。如实际使用64脚，对应型号为 R 系列封装（LQFP64）。

---

## 一、GPIO 总览

| 参数 | 值 |
|------|------|
| 封装 | LQFP64 |
| GPIO端口 | GPIOA、GPIOB、GPIOC、GPIOD |
| GPIO引脚总数 | 51 |
| GPIO供电电压 | 3.3V |
| 最大输出电流（单脚） | 25mA |
| 最大输出电流（总） | 150mA |
| 5V容忍引脚 | 大部分GPIO为5V容忍（详见数据手册FT标记） |

### 各端口引脚数

| 端口 | 引脚范围 | 数量 |
|------|----------|------|
| GPIOA | PA0 ~ PA15 | 16 |
| GPIOB | PB0 ~ PB15 | 16 |
| GPIOC | PC0 ~ PC15 | 16 |
| GPIOD | PD0 ~ PD2 | 3 |
| **合计** | | **51** |

---

## 二、电源与特殊引脚（非GPIO）

| 引脚号 | 名称 | 功能 |
|--------|------|------|
| 1 | VBAT | RTC后备电源（3.0V） |
| 3 | VDDA | ADC模拟供电（3.3V） |
| 4 | VSSA | ADC模拟地 |
| 19 | VDD_4 | 数字电源 |
| 20 | VSS_4 | 数字地 |
| 32 | VDD_1 | 数字电源 |
| 33 | VSS_1 | 数字地 |
| 48 | VDD_2 | 数字电源 |
| 49 | VSS_2 | 数字地 |
| 64 | VDD_3 | 数字电源 |
| 63 | VSS_3 | 数字地 |
| 7 | NRST | 系统复位（低有效） |
| 62 | BOOT0 | 启动模式选择 |
| 28 | BOOT1 | 启动模式选择（PB2复用） |

---

## 三、完整引脚功能表

### 3.1 GPIOA（PA0 ~ PA15）

| 引脚号 | 引脚名 | 默认功能 | 复用功能 AFIO | 重映射功能 |
|--------|--------|----------|---------------|-----------|
| 14 | PA0 | GPIO | ADC123_IN0 / WKUP | TIM2_CH1_ETR / USART2_CTS / TIM5_CH1 / ETH_MII_CRS |
| 15 | PA1 | GPIO | ADC123_IN1 | TIM2_CH2 / USART2_RTS / TIM5_CH2 / ETH_MII_RX_CLK |
| 16 | PA2 | GPIO | ADC123_IN2 | TIM2_CH3 / USART2_TX / TIM5_CH3 / ETH_MDIO |
| 17 | PA3 | GPIO | ADC123_IN3 | TIM2_CH4 / USART2_RX / TIM5_CH4 / ETH_MII_COL |
| 18 | PA4 | GPIO | ADC12_IN4 / DAC_OUT1 | SPI1_NSS / USART2_CK |
| 21 | PA5 | GPIO | ADC12_IN5 / DAC_OUT2 | SPI1_SCK |
| 22 | PA6 | GPIO | ADC12_IN6 | SPI1_MISO / TIM3_CH1 / TIM1_BKIN |
| 23 | PA7 | GPIO | ADC12_IN7 | SPI1_MOSI / TIM3_CH2 / TIM1_CH1N / ETH_MII_RX_DV |
| 24 | PA8 | GPIO | - | USART1_CK / TIM1_CH1 / I2C3_SCL / ETH_MII_TXD3 |
| 25 | PA9 | GPIO | - | USART1_TX / TIM1_CH2 / I2C3_SDA / ETH_MII_TXD2 |
| 26 | PA10 | GPIO | - | USART1_RX / TIM1_CH3 / ETH_MII_RX_ER |
| 27 | PA11 | GPIO | - | USART1_CTS / TIM1_CH4 / USBDM / CAN_RX |
| 28 | PA12 | GPIO | - | USART1_RTS / TIM1_ETR / USBDP / CAN_TX |
| 29 | PA13 | GPIO | - | JTMS / SWDIO / IR_OUT |
| 38 | PA14 | GPIO | - | JTCK / SWCLK |
| 39 | PA15 | GPIO | - | JTDI / SPI1_NSS / TIM2_CH1_ETR / I2C3_SDA |

**PA0~PA3** 的复用功能选择（重映射后）：
- TIM2_CH1~CH4 可重映射到 PA15/PB3/PB10/PB11
- TIM5_CH1~CH4（仅大容量型号，C8/R8不支持）

### 3.2 GPIOB（PB0 ~ PB15）

| 引脚号 | 引脚名 | 默认功能 | 复用功能 AFIO | 重映射功能 |
|--------|--------|----------|---------------|-----------|
| 5 | PB0 | GPIO | ADC12_IN8 | TIM3_CH3 / ETH_MII_RXD2 |
| 6 | PB1 | GPIO | ADC12_IN9 | TIM3_CH4 / ETH_MII_RXD3 |
| 28 | PB2 | GPIO | BOOT1 | -- |
| 41 | PB3 | GPIO | - | JTDO / TRACESWO / SPI1_SCK / TIM2_CH2 / I2C3_SCL |
| 42 | PB4 | GPIO | - | JNTRST / SPI1_MISO / TIM3_CH1 / I2C3_SDA |
| 43 | PB5 | GPIO | - | SPI1_MOSI / TIM3_CH2 / I2C1_SMBA / ETH_MII_PPS_OUT |
| 44 | PB6 | GPIO | - | I2C1_SCL / TIM4_CH1 / USART1_TX |
| 45 | PB7 | GPIO | - | I2C1_SDA / TIM4_CH2 / USART1_RX |
| 46 | PB8 | GPIO | - | I2C1_SCL / TIM4_CH3 / ETH_MII_TXD3 / SDIO_D4 |
| 47 | PB9 | GPIO | - | I2C1_SDA / TIM4_CH4 / ETH_MII_TX_ER / SDIO_D5 |
| 30 | PB10 | GPIO | - | I2C2_SCL / USART3_TX / TIM2_CH3 / ETH_MII_RX_ER |
| 31 | PB11 | GPIO | - | I2C2_SDA / USART3_RX / TIM2_CH4 / ETH_MII_TX_EN |
| 35 | PB12 | GPIO | - | SPI2_NSS / I2C2_SMBA / USART3_CK / TIM1_BKIN / ETH_MII_TXD0 |
| 36 | PB13 | GPIO | - | SPI2_SCK / USART3_CTS / TIM1_CH1N / ETH_MII_TXD1 |
| 37 | PB14 | GPIO | - | SPI2_MISO / USART3_RTS / TIM1_CH2N / ETH_MII_RXD0 |
| 40 | PB15 | GPIO | - | SPI2_MOSI / TIM1_CH3N / ETH_MII_RXD1 |

### 3.3 GPIOC（PC0 ~ PC15）

| 引脚号 | 引脚名 | 默认功能 | 复用功能 AFIO | 重映射功能 |
|--------|--------|----------|---------------|-----------|
| 8 | PC0 | GPIO | ADC123_IN10 | ETH_MII_MDC |
| 9 | PC1 | GPIO | ADC123_IN11 | ETH_MII_MDC |
| 10 | PC2 | GPIO | ADC123_IN12 | ETH_MII_TXD2 / SPI2_MISO |
| 11 | PC3 | GPIO | ADC123_IN13 | ETH_MII_TX_CLK / SPI2_MOSI |
| 12 | PC4 | GPIO | ADC12_IN14 | ETH_MII_TXD0 / I2C3_SDA |
| 13 | PC5 | GPIO | ADC12_IN15 | ETH_MII_TXD1 / I2C3_SCL |
| 2 | PC13 | GPIO | - | TAMPER_RTC |
| 3 | PC14 | GPIO | - | OSC32_IN |
| 4 | PC15 | GPIO | - | OSC32_OUT |

> PC6~PC12 在64脚封装中未引出（仅在100/144脚封装可用）

### 3.4 GPIOD（PD0 ~ PD2）

| 引脚号 | 引脚名 | 默认功能 | 复用功能 AFIO | 重映射功能 |
|--------|--------|----------|---------------|-----------|
| 50 | PD0 | GPIO | - | CAN_RX / USART3_CTS / ETH_MII_RXD2 |
| 51 | PD1 | GPIO | - | CAN_TX / USART3_RTS / ETH_MII_RXD3 |
| 52 | PD2 | GPIO | - | TIM3_ETR / USART3_CK / SDIO_CMD |

> PD3~PD15 在64脚封装中未引出

---

## 四、按功能分类的引脚映射

### 4.1 ADC 模拟输入

| 通道 | 引脚 | ADC组 |
|------|------|-------|
| IN0 | PA0 | ADC1/2/3 |
| IN1 | PA1 | ADC1/2/3 |
| IN2 | PA2 | ADC1/2/3 |
| IN3 | PA3 | ADC1/2/3 |
| IN4 | PA4 | ADC1/2 |
| IN5 | PA5 | ADC1/2 |
| IN6 | PA6 | ADC1/2 |
| IN7 | PA7 | ADC1/2 |
| IN8 | PB0 | ADC1/2 |
| IN9 | PB1 | ADC1/2 |
| IN10 | PC0 | ADC1/2/3 |
| IN11 | PC1 | ADC1/2/3 |
| IN12 | PC2 | ADC1/2/3 |
| IN13 | PC3 | ADC1/2/3 |
| IN14 | PC4 | ADC1/2 |
| IN15 | PC5 | ADC1/2 |

> ADC分辨率为12位，转换范围 0 ~ 3.3V（VREF+ = VDDA）

### 4.2 USART 串口

| 外设 | TX | RX | CTS | RTS | CK |
|------|----|----|-----|-----|-----|
| USART1 | PA9 / PB6 | PA10 / PB7 | PA11 | PA12 | PA8 |
| USART2 | PA2 | PA3 | PA0 | PA1 | PA4 |
| USART3 | PB10 / PC10 | PB11 / PC11 | PB13 / PC13 | PB14 / PC14 | PB12 / PC12 |

> 斜杠后为重映射引脚，PC10~PC14仅在100脚以上封装可用

### 4.3 SPI

| 外设 | NSS | SCK | MISO | MOSI |
|------|-----|-----|------|------|
| SPI1 | PA4 / PA15 | PA5 / PB3 | PA6 / PB4 | PA7 / PB5 |
| SPI2 | PB12 | PB13 | PB14 | PB15 |

### 4.4 I2C

| 外设 | SCL | SDA | SMBA |
|------|-----|-----|------|
| I2C1 | PB6 / PB8 | PB7 / PB9 | PB5 |
| I2C2 | PB10 | PB11 | PB12 |

### 4.5 TIM 定时器

| 定时器 | CH1 | CH2 | CH3 | CH4 | ETR | BKIN | CH1N | CH2N | CH3N |
|--------|-----|-----|-----|-----|-----|------|------|------|------|
| TIM1 | PA8 | PA9 | PA10 | PA11 | PA12 | PB12 / PA6 | PB13 / PA7 | PB14 | PB15 |
| TIM2 | PA0 / PA15 | PA1 / PB3 | PA2 / PB10 | PA3 / PB11 | -- | -- | -- | -- | -- |
| TIM3 | PA6 / PB4 | PA7 / PB5 | PB0 | PB1 | PD2 | -- | -- | -- | -- |
| TIM4 | PB6 | PB7 | PB8 | PB9 | -- | -- | -- | -- | -- |

### 4.6 CAN

| 引脚 | 默认 | 重映射 |
|------|------|--------|
| CAN_RX | PA11 | PB8 / PD0 |
| CAN_TX | PA12 | PB9 / PD1 |

### 4.7 USB

| 引脚 | 功能 |
|------|------|
| PA11 | USBDM（D-） |
| PA12 | USBDP（D+） |

### 4.8 调试接口

| 引脚 | SWD | JTAG |
|------|-----|------|
| PA13 | SWDIO | JTMS |
| PA14 | SWCLK | JTCK |
| PA15 | -- | JTDI |
| PB3 | -- | JTDO |
| PB4 | -- | JNTRST |

> 使用SWD调试仅需PA13、PA14两根线，释放PA15、PB3、PB4作为普通GPIO

### 4.9 DAC

| 引脚 | 功能 |
|------|------|
| PA4 | DAC_OUT1（通道1） |
| PA5 | DAC_OUT2（通道2） |

### 4.10 其他特殊功能

| 引脚 | 功能 |
|------|------|
| PA0 | WKUP（待机唤醒） |
| PC13 | TAMPER（入侵检测）/ RTC |
| PC14 | OSC32_IN（32.768KHz晶振输入） |
| PC15 | OSC32_OUT（32.768KHz晶振输出） |
| PB2 | BOOT1（启动模式选择） |

---

## 五、启动模式配置

| BOOT1 | BOOT0 | 启动区域 | 说明 |
|-------|-------|----------|------|
| x | 0 | 主Flash（0x08000000） | 正常运行模式 |
| 0 | 1 | 系统存储器（Bootloader） | 串口下载 |
| 1 | 1 | 内置SRAM | 调试用 |

> BOOT0为引脚62，BOOT1复用PB2（引脚28）。复位时采样。

---

## 六、GPIO 配置模式

每个GPIO引脚可配置为以下4种输入、4种输出模式：

### 输入模式

| 模式 | 说明 | 典型用途 |
|------|------|----------|
| 浮空输入 | 无上拉/下拉 | 外部已有驱动的信号 |
| 上拉输入 | 内部40KΩ上拉 | 按键检测（低有效） |
| 下拉输入 | 内部40KΩ下拉 | 按键检测（高有效） |
| 模拟输入 | 连接ADC/DAC | 模拟信号采集 |

### 输出模式

| 模式 | 说明 | 典型用途 |
|------|------|----------|
| 推挽输出 | 主动驱动高低电平 | LED、继电器、SPI |
| 开漏输出 | 仅拉低，需外接上拉 | I2C、电平转换 |
| 复用推挽 | 外设驱动推挽 | USART_TX、SPI_SCK、TIM_PWM |
| 复用开漏 | 外设驱动开漏 | I2C_SCL/SDA |

### 输出速度

| 速度 | 值 | 典型用途 |
|------|-----|----------|
| 低 | 2MHz | 低速信号、GPIO控制 |
| 中 | 10MHz | SPI（低速）、USART |
| 高 | 50MHz | SPI（高速）、PWM |

---

## 七、注意事项

1. **JTAG/SWD引脚占用**：上电默认PA13/PA14/PA15/PB3/PB4为JTAG功能。如需用作GPIO，需在代码中使能AFIO时钟后调用 `GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE)` 释放PB3/PB4/PA15，或 `GPIO_Remap_SWJ_Disable` 释放全部（将失去调试能力）。

2. **PC13/14/15 限制**：这三个引脚由后备域供电，作为GPIO输出时速度限制在2MHz，驱动能力弱（约3mA）。

3. **5V容忍**：标注FT（Five-volt Tolerant）的引脚可直接接5V信号。大部分GPIO为FT，但ADC输入引脚（PA0~PA7、PB0~PB1、PC0~PC5）**不是**5V容忍。

4. **ADC引脚**：用作ADC时必须配置为模拟输入模式，对应GPIO数字输入被禁用。

5. **同功能复用**：同一AFIO功能只能映射到一组引脚，不能同时使用默认和重映射引脚。

6. **时钟使能**：使用任何GPIO端口前必须先使能对应RCC时钟：
   - `RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOx, ENABLE)`
   - 使用AFIO重映射还需使能 `RCC_APB2Periph_AFIO`

---

## 八、本项目中已使用的GPIO参考

> 以下为 Shower_TFMachine 项目实际使用的GPIO分配（STM32F030），供新项目参考

| 引脚 | 功能 | 模式 |
|------|------|------|
| PA0~PA5 | ADC通道（液位/泵/电磁锁检测） | 模拟输入 |
| PA8 | TIM1_CH1（照明灯PWM） | 复用推挽 |
| PA9 | USART1_TX | 复用推挽 |
| PA10 | USART1_RX | 浮空输入 |
| PA13/PA14 | SWD调试 | 默认 |
| PB0~PB1 | ADC通道 | 模拟输入 |
| PB5 | TIM3_CH2（蜂鸣器PWM） | 复用推挽 |
| PB7 | TIM17_CH1N（电磁锁PWM） | 复用推挽 |
| PB8/PB9 | 软件 I2C（LED驱动） | 开漏输出 |
| PB12 | TIM15_CH1（液位检测PWM） | 复用推挽 |
| PB13 | TIM14_CH1（左泵PWM） | 复用推挽 |
| PB14 | TIM16_CH1（右泵PWM） | 复用推挽 |
