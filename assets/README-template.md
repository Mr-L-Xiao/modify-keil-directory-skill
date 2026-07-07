# 工程目录结构说明

本文档用于说明当前 MCU + Keil 工程的本地目录结构、各目录职责以及 Keil 工程分组方式。

## 本地目录结构

```text
App/
  function/
  User_Config/
  main.c
BSP/
  board/
  devices/
Common/
  User_System/
  utilities/
Docs/
Drivers/
Output/
  Listings/
  Objects/
  Bin/
Project/
  Keil-MDK/
```

若使用 RTOS，则额外增加：

```text
App/tasks/
Middlewares/FreeRTOS/
```

## 各目录作用

- `App/`
  - 应用层代码。
  - `main.c` 作为程序入口。
  - `function/` 存放业务功能代码，也可以放包含业务逻辑的外设开发代码。
  - `tasks/` 仅在使用 RTOS 时存在，存放任务代码。
  - `User_Config/` 存放用户自定义配置文件，可包含运行时和编译时的全局配置 `.c` 和 `.h` 文件。

- `BSP/`
  - 板级支持包。
  - `board/` 存放板级外设支持代码，例如 `bsp_gpio.c`、`bsp_adc.c`。
  - `devices/` 存放用户自己写的外设硬件控制代码，不包含业务逻辑，例如 `led`、`motor`、`encoder`、`sensor`。

- `Common/`
  - 工程公共系统胶水与资源代码。
  - `User_System/` 存放 `xx_it.c/h`、`interrupts_xx32xx.c/h`、`systick.c/h` 等系统级公共文件。
  - `utilities/` 存放工具类代码，例如 `crc16.c/h`、`ring_buffer.c/h`。

- `Docs/`
  - 存放工程文档。

- `Drivers/`
  - 存放芯片厂商/CMSIS/底层驱动与设备支持文件。
  - 例如 `CMSIS/`、`xx32xx_Driver/`、`xx32xx_System/`。

- `Middlewares/`
  - 存放第三方中间件源码。
  - 例如 `FreeRTOS/`。

- `Output/`
  - 存放编译产物。
  - `Objects/` 存放 `.o/.axf/.hex` 等文件。
  - `Listings/` 存放 `.map/.lst` 等分析文件。
  - `Bin/` 存放 `.bin` 文件。

- `Project/`
  - 存放工程文件。
  - `Keil-MDK/` 存放 `uvprojx`、启动文件、`.sct` 等 Keil 相关文件。

## 系统相关文件建议放置位置

- `startup_xxx.s`、`.sct`
  - 放在 `Project/Keil-MDK/`

- `hc32l19x.h`、`stm32f1xx.h`、`core_cm*.h`、`system_xxx.c`
  - 放在 `Drivers/xx32xx_System/` 或 `Drivers/CMSIS/`

- `xx_it.c`、`xx_it.h`
  - 放在 `Common/User_System/`

- `systick.c`、`systick.h`
  - 放在 `Common/User_System/`

## Keil 工程分组建议

Keil 分组是虚拟分组，不要求必须与本地目录同名。

RTOS 工程可参考：

```text
项目名
├─System
├─App/User
├─App/tasks
├─App/function
├─Common
├─BSP/board
├─BSP/devices
├─Drivers/xx32xx_Driver
├─Middlewares/FreeRTOS
└─...
```

无 RTOS 工程可参考：

```text
项目名
├─System
├─App/User
├─App/function
├─Common
├─BSP/board
├─BSP/devices
├─Drivers/xx32xx_Driver
└─...
```

## 编译输出建议

- `Objects/`：存放 `.o/.axf/.hex` 等文件
- `Listings/`：存放 `.map/.lst` 等文件
- `Bin/`：存放 `fromelf` 转换得到的 `.bin` 文件

建议通过 `.gitignore` 忽略 `Objects/`、`Listings/` 等中间产物，并按需要决定是否保留 `Bin/` 下的 `.bin` 文件。
