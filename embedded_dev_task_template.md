# 嵌入式软件开发任务模板

> 基于 STM32F030 淋浴投放器项目生成的通用任务分解模板
> 项目：Shower_TFMachine（淋浴投液器MCU软件）
> 日期：2026-05-08

---

## 一、软件架构概述

### 分层结构
```
┌─────────────────────────────────────────────┐
│              应用层 (MYUSER)                  │
│  主状态机 │ 通信协议 │ 业务逻辑 │ 产测/显示   │
├─────────────────────────────────────────────┤
│              驱动层 (MYDRIVER)                │
│  UART │ ADC │ TIM_PWM │ GPIO │ Flash │ Queue │
├─────────────────────────────────────────────┤
│              Core (STM32 LL库)               │
│  启动文件 │ 系统时钟 │ 中断向量 │ main框架    │
└─────────────────────────────────────────────┘
```

### 核心设计模式
1. **全局设备结构体**：`tMyDev TMyDev` 集中管理所有状态
2. **非阻塞时间驱动状态机**：`get_local_absolute_time()` 超时比较，禁止 delay()
3. **主状态机 + 子状态机**：`soft_state` 枚举分发到各业务模块的子状态机
4. **TLV通信协议**：AA帧头 + 设备类型 + 帧序列 + 消息类型 + TLV数据 + 校验
5. **Flash参数持久化**：阈值、SN、混液标志、基准峰峰值等关键参数掉电保存

---

## 二、任务分解（10个阶段，32个任务）

---

### P1：设备数据模型（4个任务）

#### T1.1 定义软件状态枚举和宏常量
- **文件**：`MYUSER/mydev.h`
- **依赖**：无
- **输入**：产品需求文档中的工作模式、工作状态定义
- **输出**：7个枚举（soft_state_enum, soft_mode_enum, work_state_enum, beep_state_enum, throw_process_state_enum, throw_auto_state_enum, segment_code）+ ~50个宏定义
- **验收标准**：
  - 所有枚举值覆盖产品所有工作状态
  - 宏常量按功能分组（定时时间、检测参数、引脚电平、LED状态等）
  - 编译无错误
- **具体内容**：
  ```
  soft_state_enum: SOFT_INIT / SOFT_IDLE / SOFT_THROW / SOFT_THROW_AUTO / SOFT_THROW_AUTO_2 / SOFT_FCT_TEST / SOFT_OPEN_DOOR
  soft_mode_enum: SOFT_MODE_ORDER / SOFT_MODE_FREE / SOFT_MODE_EMPTY
  work_state_enum: WORK_STATE_IDLE / WORK_STATE_OPEN_ELEC / WORK_STATE_THROW / WORK_STATE_CLOSE_ELEC
  throw_process_state_enum: 14个状态（按键检测→左投→右投→左液位检测→左补投→右液位检测→右补投→强制退出→错误→退出）
  throw_auto_state_enum: 9个状态（左自动→左检测→右自动→右检测→退出）
  ```

#### T1.2 定义故障位和标志位结构体
- **文件**：`MYUSER/mydev.h`
- **依赖**：T1.1
- **输入**：硬件故障类型列表、通信标志需求
- **输出**：t_err_bit（10个故障位）、t_flag_bit（通信标志位）、u32_to_byte联合体
- **验收标准**：
  - 故障位覆盖所有需要检测的硬件异常
  - u32_to_byte支持按位操作和整体读写
- **具体内容**：
  ```
  t_err_bit: 左/右探针短接、上盖未关、上盖异常开启、电磁锁使能/失能故障、左/右泵使能/失能故障
  t_flag_bit: L3通信正常标志
  u32_to_byte: t_bit + byte[4] + u32_int 联合体
  ```

#### T1.3 定义LED和设备主结构体
- **文件**：`MYUSER/mydev.h`
- **依赖**：T1.1, T1.2
- **输入**：产品功能需求中的所有状态变量
- **输出**：tLedSegment（LED指示灯结构体）、tMyDev（主设备结构体，~80个字段）
- **验收标准**：
  - tMyDev包含所有业务逻辑需要的状态变量
  - 左右罐字段对称设计，便于后续统一处理
  - 结构体字段命名清晰、有注释
- **具体内容**：
  ```
  tMyDev主要分组：
  - 系统状态：soft_state, soft_mode, work_state, m_iap_flag
  - 订单处理：throw_process_state, order_flag
  - 左罐参数：投放标志/次数/频率/时间、液位检测状态/AD缓存/峰峰值/阈值/混液标志/补偿值/基准值
  - 右罐参数：与左罐对称
  - 通信参数：server_no, mcu_cmd_no, heart_cnt
  - 外设状态：LED/锁/门、蜂鸣器、AD检测缓存
  - 故障状态：err_state
  ```

#### T1.4 实现设备初始化和模式记录
- **文件**：`MYUSER/mydev.c`
- **依赖**：T1.3
- **输入**：tMyDev结构体定义
- **输出**：`TMyDev`全局实例、`mydev_init()`初始化函数、`dev_soft_mode_change_record()`模式切换记录
- **验收标准**：
  - 所有字段初始化为安全的默认值
  - soft_state = SOFT_INIT
  - 全局变量 TMyDev 可被其他模块 extern 引用
  - 编译无错误
- **具体内容**：
  ```c
  tMyDev TMyDev;
  void mydev_init(tMyDev *pMyDev) {
      // 所有字段清零
      // soft_state = SOFT_INIT
      // 读取Flash参数填充阈值、混液标志、基准峰峰值
      // 初始化LED默认状态
  }
  ```

---

### P2：调试输出模块（1个任务）

#### T2.1 实现RTT调试输出
- **文件**：`MYUSER/my_rtt_debug.h/c`
- **依赖**：T1.1
- **输入**：SEGGER RTT库
- **输出**：带时间戳的hex打印、调试开关宏
- **验收标准**：
  - 支持 hex 数组打印，带时间戳
  - 通过宏控制打印开关， Release 编译时零开销
  - 编译无错误
- **具体内容**：
  ```c
  // 打印发送数据（带时间戳）
  void rtt_print_tx_data(uint8_t *data, uint16_t len);
  // 打印接收数据（带时间戳）
  void rtt_print_rx_data(uint8_t *data, uint16_t len);
  // 宏控制开关
  #define PRINT_TX_ON/OFF
  #define PRINT_RX_ON/OFF
  ```

---

### P3：通信协议模块（10个任务）

#### T3.1 定义协议常量和命令接口
- **文件**：`MYUSER/command.h`
- **依赖**：T1.3
- **输入**：通信协议文档
- **输出**：帧格式偏移宏（14个）、消息类型宏（~20个）、TLV关键字宏（~30个）、版本号宏、~40个函数声明
- **验收标准**：
  - 所有协议常量与协议文档一致
  - 函数声明覆盖所有命令处理和响应
  - 编译无错误
- **具体内容**：
  ```
  帧格式：AA [长度] [设备类型CD] [2字节机器帧号] [2字节服务器帧号] [消息类型] [TLV数据] [校验]
  消息类型：0x02控制 / 0x03查询 / 0x04状态上报 / 0x07SN查询 / 0xE0产测 / 0xDC心跳
  TLV关键字：0xC2授权 / 0xB1结单 / 0xD0绑定 / 0xD1排空 / 0xD3开锁 / 0xDA自检 等
  版本号：功能码0x40 / 年月日 / 模块类型0x02(4G) / 主版本01 / 子版本0000
  ```

#### T3.2 实现帧校验和通用响应函数
- **文件**：`MYUSER/command.c`（基础部分）
- **依赖**：T3.1
- **输入**：帧格式定义
- **输出**：`check_frame()`帧校验、`respond_cmd()`通用帧构建、`float_to_bcd_format()`浮点转BCD
- **验收标准**：
  - check_frame 正确验证帧头、长度、校验和
  - respond_cmd 按协议格式组装完整帧并通过UART发送
  - 帧序列号自动递增
  - 编译无错误
- **具体内容**：
  ```c
  // 校验帧头AA + 长度 + 校验和
  uint8_t check_frame(uint8_t *ptr, uint16_t len);

  // 通用响应帧构建：填充帧头→设备类型→帧序列→消息类型→数据体→校验→发送
  void respond_cmd(uint8_t msg_type, uint8_t sub_type, uint8_t *tlv_data,
                   uint16_t tlv_len, tMyDev *pMyDev, uint8_t send_type);
  ```

#### T3.3 实现控制命令响应（0x02）
- **文件**：`MYUSER/command.c`（控制响应部分）
- **依赖**：T3.2
- **输入**：控制命令TLV类型
- **输出**：10个respond_02CAxx_cmd函数（C2授权/B1结单/D0绑定/D1排空/D2强制空闲/D3开锁/D4照明亮度/D6自动排液/D7照明开关/F4模式切换）
- **验收标准**：
  - 每个函数正确组装响应帧
  - 响应结果码正确
  - 编译无错误
- **具体内容**：
  ```c
  respond_02CAC2_cmd(reselut, pMyDev, send_type)  // 授权响应
  respond_02CAD0_cmd(reselut, pMyDev, send_type)  // 绑定响应
  respond_02CAD1_cmd(reselut, pMyDev, send_type)  // 排空响应
  // ... 共10个
  ```

#### T3.4 实现查询命令响应（0x03）
- **文件**：`MYUSER/command.c`（查询响应部分）
- **依赖**：T3.2
- **输入**：查询命令TLV类型
- **输出**：11个respond_03CAxx_cmd函数（D1排空查询/D4亮度/D6自动排液/D8阈值/D9液位/DA自检/DB-SN/DC排空/DD补偿/DE阈值查询/DF补偿查询）
- **验收标准**：
  - 查询响应携带正确的设备当前状态数据
  - 编译无错误
- **具体内容**：
  ```c
  respond_03CAD8_cmd(pMyDev, send_type)  // 查询阈值，返回所有阈值参数
  respond_03CADA_cmd(pMyDev, send_type, result)  // 自检结果上报
  // ... 共11个
  ```

#### T3.5 实现状态上报命令（0x04/0x05）
- **文件**：`MYUSER/command.c`（状态上报部分）
- **依赖**：T3.2
- **输入**：上报消息类型定义
- **输出**：17个respond_04/05DAxx_cmd函数（C1设备信息/C2缺液/C3-C6-ADC状态/C7结果/C8液位/C9设备状态/CA工作状态/CB事件）
- **验收标准**：
  - 上报数据格式符合协议
  - 包含设备当前状态快照
  - 编译无错误
- **具体内容**：
  ```c
  respond_04DAC1_cmd(pMyDev, send_type)  // 上报投液器信息
  respond_04DAC8_cmd(pMyDev, send_type)  // 上报液位ADC值（精度1mV）
  respond_04DACA_cmd(pMyDev, work_state, send_type)  // 上报工作状态
  // ... 共17个
  ```

#### T3.6 实现特殊命令响应（心跳/SN/版本/OTA/产测）
- **文件**：`MYUSER/command.c`（特殊命令部分）
- **依赖**：T3.2
- **输入**：特殊命令定义
- **输出**：respond_dc_cmd(心跳)、respond_07_cmd(SN查询)、respond_11_cmd(写SN)、respond_87_cmd(版本)、respond_93_cmd(OTA)、respond_e4_cmd(产测结果)、respond_e0_cmd(产测开始)
- **验收标准**：
  - 心跳响应定时触发
  - SN读写正确操作Flash
  - 版本响应包含编译日期
  - 编译无错误

#### T3.7 实现控制命令TLV解析（0x02 CA多TLV）
- **文件**：`MYUSER/command.c`（TLV解析部分）
- **依赖**：T3.3, T3.4
- **输入**：控制命令格式
- **输出**：`parse_02ca_multi_tlv()` 函数，处理10种TLV类型
- **验收标准**：
  - 正确解析TLV数据（Type-Length-Value循环读取）
  - 每种TLV类型调用对应的业务处理函数
  - 处理完成后发送对应响应
  - 编译无错误
- **具体内容**：
  ```
  TLV解析流程：
  while(偏移 < tlv_len) {
      type = buff[offset++]
      length = buff[offset++]
      switch(type) {
          case 0xC2: 解析授权参数 → 进入订单模式
          case 0xB1: 解析结单参数 → 退出订单模式
          case 0xD0: 绑定/解绑处理
          case 0xD1: 触发排空
          case 0xD3: 触发开锁
          case 0xD4: 设置照明亮度
          case 0xD6: 触发自动排液(液体激活)
          case 0xD7: 照明灯开关
          case 0xD8: 查询阈值
          case 0xF4: 免费/收费模式切换
      }
      offset += length
  }
  ```

#### T3.8 实现查询命令TLV解析（0x03 CA多TLV）
- **文件**：`MYUSER/command.c`（查询TLV解析部分）
- **依赖**：T3.4, T3.5
- **输入**：查询命令格式
- **输出**：`parse_03ca_multi_tlv()` 函数，处理14种TLV类型
- **验收标准**：
  - 正确解析查询请求
  - 查询结果通过响应函数上报
  - 编译无错误

#### T3.9 实现命令总解析入口
- **文件**：`MYUSER/command_parse.c/h`
- **依赖**：T3.7, T3.8
- **输入**：完整UART接收帧
- **输出**：`cmd_parse()` 主解析函数、`fct_cmd_deal()` 产测命令分发、`soft_reset()` 软件复位
- **验收标准**：
  - 按消息类型switch分发到对应处理函数
  - 支持全部消息类型（0x02~0xE4）
  - 帧序列号正确保存和回显
  - 编译无错误
- **具体内容**：
  ```c
  void cmd_parse(uint8_t *buff, uint16_t len, tMyDev *pMyDev, uint8_t send_type) {
      uint8_t msg_type = buff[PB_MAIN_MESSAGE_TYPE_OFFSET];
      switch(msg_type) {
          case 0x02: parse_02ca_multi_tlv(...)
          case 0x03: parse_03ca_multi_tlv(...)
          case 0x07: respond_07_cmd(...)
          case 0x0D: // 网络同步通知
          case 0x11: respond_11_cmd(...)
          case 0x61: // 时间同步
          case 0x87: respond_87_cmd(...)
          case 0x93: respond_93_cmd(...)
          case 0xDC: // 心跳
          case 0xE0: fct_cmd_deal(...)
          // ...
      }
  }
  ```

#### T3.10 实现UART接收状态机
- **文件**：`MYUSER/factory.c/h`
- **依赖**：T3.9
- **输入**：UART DMA接收中断
- **输出**：`uart_cmd_parse()` 接收状态机（等待0xAA→读取长度→接收完整帧→调用cmd_parse）
- **验收标准**：
  - 状态机正确处理字节流
  - 支持断帧、超时处理
  - 接收完整帧后调用cmd_parse
  - 编译无错误
- **具体内容**：
  ```
  接收状态机：
  STATE_WAIT_AA: 等待0xAA帧头
  STATE_WAIT_LEN: 读取帧长度
  STATE_RECV_DATA: 接收剩余字节
  STATE_COMPLETE: 校验→调用cmd_parse→重置状态机
  ```

---

### P4：硬件初始化与传感器检测（6个任务）

#### T4.1 实现硬件初始化函数
- **文件**：`MYUSER/report_up.c`（初始化部分）
- **依赖**：T1.4, 驱动层完成
- **输入**：驱动层API
- **输出**：`hardware_init()` 调用所有驱动层初始化（GPIO/UART/ADC/TIM/DMA/Flash）+ 调用mydev_init
- **验收标准**：
  - 硬件初始化顺序正确（先GPIO再外设）
  - mydev_init正确读取Flash参数
  - 所有外设初始化成功
  - 编译无错误

#### T4.2 实现数学辅助函数
- **文件**：`MYUSER/report_up.c`（辅助函数部分）
- **依赖**：T1.1
- **输入**：检测算法需求
- **输出**：冒泡排序、去除极值、求平均值、求峰峰值、绝对值比较等数学工具函数
- **验收标准**：
  - 排序算法正确（升序）
  - 去除N个最大最小值后求平均
  - 编译无错误

#### T4.3 实现静态液位检测算法
- **文件**：`MYUSER/report_up.c`（静态检测部分）
- **依赖**：T4.2
- **输入**：ADC采样数据，检测参数
- **输出**：`level_static_check()` 静态液位检测函数（采样→排序→去极值→求峰峰值→阈值判断）
- **验收标准**：
  - 采样100个ADC数据
  - 冒泡排序后去除前4后4共8个极值
  - 峰峰值 = 最大值 - 最小值
  - 根据峰峰值判断：>短路阈值→短路，>混液阈值→混液，否则→正常
  - 编译无错误
- **具体内容**：
  ```
  静态检测流程：
  1. PWM激励输出 → 等待250ms稳定
  2. ADC连续采样100次
  3. 冒泡排序（升序）
  4. 去除前4个最小值和后4个最大值（共8个极值）
  5. 计算峰峰值 = buff[EXCEPTION_LEVEL_NUMBER] - buff[SAMPLE_LEVEL_NUMBER-EXCEPTION_LEVEL_NUMBER]
  6. 阈值判断：
     - 峰峰值 > 短路阈值(0xFF对应范围) → LEVEL_STATE_SHORT_CIRCLE
     - 峰峰值 > 混液阈值 → LEVEL_STATE_MIXED
     - 峰峰值 <= 20mV → LEVEL_STATE_SHORT_LIQUID (缺液)
     - 否则 → LEVEL_STATE_NO_SHORT_LIQUID (不缺液)
  ```

#### T4.4 实现动态液位检测算法
- **文件**：`MYUSER/report_up.c`（动态检测部分）
- **依赖**：T4.3
- **输入**：ADC采样数据，基准峰峰值，混液阈值
- **输出**：`level_dynamic_check()` 动态液位检测函数（计算△峰峰值，与基准和θ阈值对比）
- **验收标准**：
  - 计算当前峰峰值与基准峰峰值的差值(△峰峰值)
  - △峰峰值 < θ阈值 → 不缺液
  - △峰峰值 >= θ阈值 → 可能缺液，进入补偿判断
  - 编译无错误
- **具体内容**：
  ```
  动态检测流程：
  1. 计算当前峰峰值（同静态检测步骤1-5）
  2. △峰峰值 = 当前峰峰值 - 基准峰峰值
  3. 判断：
     - △峰峰值 < 混液阈值 → 液位正常
     - △峰峰值 >= 混液阈值 且 △峰峰值 < θ阈值 → 可能缺液，标记
     - △峰峰值 >= θ阈值 → 缺液，需要补偿投放
  4. 更新基准峰峰值（如果条件满足）
  ```

#### T4.5 实现电磁锁和蠕动泵检测
- **文件**：`MYUSER/report_up.c`（硬件检测部分）
- **依赖**：T4.2
- **输入**：ADC采样数据，故障阈值
- **输出**：`elec_lock_check()`电磁锁检测、`pump_check()`蠕动泵检测，检测到故障时上报
- **验收标准**：
  - 电磁锁：采样20次→排序→去极值→判断电压是否在预期范围
  - 蠕动泵：同上方法，检测泵使能/失能时的电流反馈
  - 故障时设置err_state位并上报
  - 编译无错误

#### T4.6 实现蠕动泵控制和蜂鸣器控制
- **文件**：`MYUSER/report_up.c`（控制输出部分）
- **依赖**：T4.5
- **输入**：驱动层PWM API
- **输出**：`pump_control()`泵控制（启停+故障检测）、`beep_ctrl_process()`蜂鸣器状态机（阻塞式+非阻塞式）
- **验收标准**：
  - 泵控制支持左右侧独立启停
  - 启停时检测电流反馈，异常上报故障
  - 蜂鸣器支持多种提示音模式（短响、长响、三连响）
  - 蜂鸣器状态机非阻塞运行
  - 编译无错误
- **具体内容**：
  ```
  beep_state_enum:
  BEEP_STATE_IDLE → 空闲
  BEEP_STATE_300MS_2_ON1 → 300ms后响100ms
  BEEP_STATE_300MS_2_OFF1 → 静音100ms
  BEEP_STATE_300MS_2_ON2 → 响200ms
  BEEP_STATE_300MS_2_OFF2 → 静音200ms
  BEEP_STATE_300MS_2_ON3 → 响300ms
  ```

---

### P5：投放流程状态机（5个任务）

#### T5.1 实现订单投放主状态机
- **文件**：`MYUSER/throw_process.c`（订单投放部分）
- **依赖**：T4.3, T4.4, T4.6
- **输入**：throw_process_state_enum 定义
- **输出**：`throw_process()` 12状态的主投放状态机
- **验收标准**：
  - 状态机按：按键检测→左投→右投→左液位检测→左补投→右液位检测→右补投→退出 流程运行
  - 每个状态超时自动跳到下一状态或错误状态
  - 支持强制退出
  - 编译无错误
- **具体内容**：
  ```
  状态流转：
  PRESS_CHECK → LEFT_GO → RIGHT_GO →
  LEFT_LEVEL_START_CHECK → LEFT_LEVEL_START_COMP_THROW → LEFT_LEVEL_START_PUMP → LEFT_LEVEL_END_CHECK →
  RIGHT_LEVEL_START_CHECK → RIGHT_LEVEL_START_COMP_THROW → RIGHT_LEVEL_START_PUMP → RIGHT_LEVEL_END_CHECK →
  STATE_EXIT / FORCE_EXIT / ERR

  关键逻辑：
  - 投放次数根据订单参数决定
  - 液位检测后根据结果决定是否补投
  - 补投次数根据 comp_value 动态计算
  ```

#### T5.2 实现自动投放状态机（液体激活）
- **文件**：`MYUSER/throw_process.c`（自动投放部分）
- **依赖**：T5.1
- **输入**：throw_auto_state_enum 定义
- **输出**：`soft_auto_throw_process()` 9状态的自动投放状态机（含液位检测）
- **验收标准**：
  - 左侧自动投放→左侧液位检测→右侧自动投放→右侧液位检测→退出
  - 支持液体激活场景
  - 编译无错误
- **具体内容**：
  ```
  状态流转：
  LEFT → LEFT_AUTO_LEVEL_CHECK → LEFT_AUTO_PREPARE_LEVEL_CHECK → LEFT_AUTO_START_LEVEL_CHECK →
  RIGHT → RIGHT_AUTO_LEVEL_CHECK → RIGHT_AUTO_PREPARE_LEVEL_CHECK → RIGHT_AUTO_START_LEVEL_CHECK →
  EXIT
  ```

#### T5.3 实现自动排空状态机（产测用）
- **文件**：`MYUSER/throw_process.c`（排空部分）
- **依赖**：T5.2
- **输入**：排空时间参数
- **输出**：`soft_auto_throw_process_2()` 直接排空状态机（不进行液位检测）
- **验收标准**：
  - 按设定时间直接排空，不做液位检测
  - 排空完成后上报结果
  - 编译无错误

#### T5.4 实现基准峰峰值更新算法
- **文件**：`MYUSER/throw_process.c`（基准更新部分）
- **依赖**：T5.1, T4.4
- **输入**：当前峰峰值、基准峰峰值、混液标志
- **输出**：`update_base_peak_to_peak()` 基准峰峰值更新逻辑
- **验收标准**：
  - 投放成功后根据当前峰峰值更新基准
  - 混液标志影响更新策略
  - 更新后写入Flash持久化
  - 编译无错误
- **具体内容**：
  ```
  更新策略：
  1. 首次投放：当前峰峰值作为基准写入Flash
  2. 后续投放：
     - 未混液：当前峰峰值更新基准
     - 已混液：不更新基准（混液状态下峰峰值不可靠）
  3. 基准变化超过阈值时才更新Flash（减少Flash擦写次数）
  ```

#### T5.5 实现补偿投放逻辑
- **文件**：`MYUSER/throw_process.c`（补偿投放部分）
- **依赖**：T5.1, T5.4
- **输入**：comp_value、当前峰峰值、基准峰峰值
- **输出**：补偿投放次数计算和执行逻辑
- **验收标准**：
  - 根据comp_value和峰峰值差值计算补偿次数
  - 补偿投放后重新检测液位
  - 达到最大补偿次数后停止
  - 编译无错误
- **具体内容**：
  ```
  补偿逻辑：
  1. 计算 △峰峰值 = 当前峰峰值 - 基准峰峰值
  2. 补偿次数 = △峰峰值 / comp_value（向上取整）
  3. 执行补偿投放
  4. 重新检测液位
  5. 如仍缺液且未达到最大补偿次数 → 继续补偿
  6. 达到最大次数 → 上报缺液
  ```

---

### P6：开锁流程（1个任务）

#### T6.1 实现电磁锁控制状态机
- **文件**：`MYUSER/open_door_process.c/h`
- **依赖**：T4.5, T4.6
- **输入**：锁控制需求
- **输出**：`open_door_process()` 5状态锁控制状态机 + 故障检测上报
- **验收标准**：
  - 状态机：START→STEP1(输出)→CHECK_ELEC(检测反馈)→END(关闭输出)→FORCE_EXIT
  - 电磁锁输出持续800ms后关闭
  - 输出后1s检测电压反馈判断是否开锁成功
  - 故障时上报err_state并蜂鸣提示
  - 编译无错误
- **具体内容**：
  ```
  状态流转：
  START → 启动TIM17输出PWM → 等待800ms
  STEP1 → 关闭PWM输出 → 等待1s
  CHECK_ELEC → ADC检测电磁锁反馈电压 → 判断成功/失败
  END → 上报结果
  FORCE_EXIT → 异常退出，上报故障
  ```

---

### P7：FCT产测模块（1个任务）

#### T7.1 实现FCT产测流程
- **文件**：`MYUSER/fct.c/h`
- **依赖**：T4.6, T6.1, T5.1
- **输入**：产测需求文档
- **输出**：`fct_e0()` 产测入口、11个产测项目函数、产测结果上报
- **验收标准**：
  - 支持11项产测：SN烧录、液位检测、左/右蠕动泵、电磁锁、MCU版本、上盖检测、左/右按键、LED数码管、照明灯
  - 每项测试有30s超时
  - 产测结果通过0xE4上报
  - 编译无错误
- **具体内容**：
  ```
  11项产测：
  1. SN烧录测试
  2. 静态液位检测测试（左/右）
  3. 左蠕动泵运行测试
  4. 右蠕动泵运行测试
  5. 电磁锁开合测试
  6. MCU软件版本读取
  7. 上盖开合检测测试
  8. 左按键测试
  9. 右按键测试
  10. LED数码管显示测试
  11. 照明灯亮度测试
  ```

---

### P8：LED显示模块（2个任务）

#### T8.1 实现软件I2C LED驱动
- **文件**：`MYUSER/led_segment.c`（驱动部分）
- **依赖**：驱动层GPIO
- **输入**：LED驱动芯片规格书（TM1650类）
- **输出**：软件I2C时序、LED寄存器读写、段码表、数码管刷新函数
- **验收标准**：
  - I2C时序符合芯片规格
  - 支持亮度调节
  - 按键灯和液位灯独立控制
  - 编译无错误

#### T8.2 实现LED业务逻辑
- **文件**：`MYUSER/led_segment.c`（业务部分）
- **依赖**：T8.1
- **输入**：LED显示需求
- **输出**：状态灯更新（红/白/灭/闪烁）、数码管显示逻辑（版本号/错误码/正常显示）、`led_segment_control()`总控制函数
- **验收标准**：
  - 不同soft_state下LED显示不同状态
  - 支持200ms快闪和2s慢闪
  - 缺液/故障有明确LED指示
  - 编译无错误
- **具体内容**：
  ```
  LED显示规则：
  - SOFT_IDLE: 白灯常亮（正常）或红灯（缺液）
  - SOFT_THROW: 投放中闪烁
  - SOFT_FCT_TEST: 数码管显示产测状态
  - SOFT_OPEN_DOOR: 开锁提示
  - 缺液: 红灯闪烁
  - 故障: 红灯常亮
  ```

---

### P9：主流程框架（1个任务）

#### T9.1 实现主状态机框架
- **文件**：`MYUSER/mainprocess.c/h`
- **依赖**：所有P1-P8任务
- **输入**：soft_state_enum 定义
- **输出**：`mainprocess_parse()` 主状态机（按soft_state分发到各业务函数）、周期性任务调度（LED控制200ms/500ms、心跳30s）
- **验收标准**：
  - 主状态机正确分发：
    - SOFT_INIT → 硬件初始化 → 跳转SOFT_IDLE
    - SOFT_IDLE → 周期检测、命令等待
    - SOFT_THROW → throw_process()
    - SOFT_THROW_AUTO → soft_auto_throw_process()
    - SOFT_THROW_AUTO_2 → soft_auto_throw_process_2()
    - SOFT_FCT_TEST → fct_e0()
    - SOFT_OPEN_DOOR → open_door_process()
  - 周期任务按时间片调度，非阻塞
  - main()的while(1)循环中正确调用7个子函数
  - 编译无错误
- **具体内容**：
  ```c
  // main.c while(1)循环
  while(1) {
      mainprocess_parse(&TMyDev);   // 主状态机
      uart_cmd_parse(&TMyDev);      // UART接收解析
      regular_led_control(&TMyDev); // LED周期控制
      regular_heart_deal(&TMyDev);  // 心跳处理
      regular_up_open_check_and_report(&TMyDev);  // 上盖检测
      beep_ctrl_process(&TMyDev);   // 蜂鸣器状态机
      rtt_deal();                   // RTT调试
  }
  ```

---

### P10：集成测试（1个任务）

#### T10.1 系统集成与功能验证
- **文件**：全部
- **依赖**：T9.1
- **输入**：完整编译的项目
- **输出**：集成测试报告
- **验收标准**：
  - 编译0 Error 0 Warning
  - 下位机与上位机通信正常（心跳、命令、上报）
  - 订单流程完整跑通（授权→投放→液位检测→补偿→结单）
  - 产测流程完整跑通
  - 开锁流程正常
  - 异常场景处理正确（缺液、短路、超时）
- **具体内容**：
  ```
  测试清单：
  □ 上电初始化正常，LED显示正确
  □ 4G模块通信建立，心跳正常
  □ 授权命令（0xC2）→ 进入订单模式
  □ 投放命令 → 左/右泵运行 → 液位检测 → 补偿投放
  □ 结单命令（0xB1）→ 退出订单模式
  □ 排空命令（0xD1）→ 排空流程
  □ 开锁命令（0xD3）→ 电磁锁动作
  □ 产测命令（0xE0）→ 11项产测
  □ 缺液检测 → 上报缺液状态
  □ 故障检测 → 上报故障码
  □ 参数掉电保存验证
  ```

---

## 三、任务依赖关系图

```
P1(T1.1~T1.4) ─── 基础数据模型
    │
    ├── P2(T2.1) ─── RTT调试
    │
    ├── P3(T3.1~T3.10) ─── 通信协议
    │       T3.1 → T3.2 → T3.3/T3.4/T3.5/T3.6 → T3.7/T3.8 → T3.9 → T3.10
    │
    ├── P4(T4.1~T4.6) ─── 硬件检测
    │       T4.1 → T4.2 → T4.3 → T4.4 → T4.5 → T4.6
    │
    ├── P5(T5.1~T5.5) ─── 投放流程
    │       T5.1 → T5.2 → T5.3
    │       T5.1 + T5.4 → T5.5
    │
    ├── P6(T6.1) ─── 开锁流程
    │
    ├── P7(T7.1) ─── FCT产测
    │
    ├── P8(T8.1~T8.2) ─── LED显示
    │       T8.1 → T8.2
    │
    ├── P9(T9.1) ─── 主流程框架
    │
    └── P10(T10.1) ─── 集成测试
```

**建议开发顺序**：P1 → P2 → P4 → P3 → P5 → P6 → P7 → P8 → P9 → P10

> 注：P3和P4可以并行开发，P5/P6/P7/P8可以并行开发（都依赖P4但不互相依赖）

---

## 四、模板使用说明

### 新项目使用步骤
1. **分析产品需求**，确定状态枚举、工作模式、TLV命令类型
2. **按P1-P10阶段顺序**，逐个任务实现
3. **每个任务**按"文件→依赖→输入→输出→验收标准"模板填写项目具体内容
4. **先驱动后应用**：确保驱动层API全部就绪后再开始应用层
5. **增量编译**：每完成一个任务立即编译验证
6. **非阻塞原则**：所有状态机使用时间比较，禁止delay()阻塞

### AI辅助开发建议
- 每个任务可以作为一个独立的AI对话轮次
- 给AI提供：任务描述 + 依赖的头文件 + 驱动层API声明
- AI输出：完整的.c/.h文件实现
- 验证后提交，再进入下一个任务
