# Mango_RVOS

Mango_RVOS 是基于 RISC-V 架构的简易裸机操作系统，运行于 QEMU virt 虚拟硬件平台，仅支持单个 hart。项目主要由内核源代码、头文件、汇编文件、链接脚本和构建文件组成，实现内核启动、物理内存管理、任务调度、异常与中断处理、定时器管理以及系统调用等功能。

## 项目目录结构

```text
Mango_RVOS/
├── .gitignore        # Git 忽略规则
├── CLAUDE.md         # AI 辅助开发说明
└── os/               # 操作系统源码目录
    ├── Makefile      # 项目构建与运行配置
    ├── os.ld         # 链接脚本与内存布局
    ├── start.S       # 内核启动与初始化
    ├── entry.S       # 上下文保存与恢复
    ├── mem.S         # 内存布局符号定义
    ├── kernel.c      # 内核初始化与入口
    ├── kernel.h      # 内核公共接口声明
    ├── types.h       # 基本数据类型定义
    ├── riscv.h       # RISC-V CSR 访问接口
    ├── platform.h    # 硬件平台参数定义
    ├── uart.c        # UART 串口驱动
    ├── printf.c      # 格式化输出实现
    ├── page.c        # 物理内存管理
    ├── sched.c       # 任务管理与调度
    ├── trap.c        # 异常与中断处理
    ├── plic.c        # PLIC 中断控制器
    ├── timer.c       # 硬件与软件定时器
    ├── lock.c        # 锁机制实现
    ├── syscall.c     # 系统调用处理
    ├── syscall.h     # 系统调用号定义
    ├── usys.S        # 用户态系统调用入口
    └── user.c        # 用户任务与示例程序
