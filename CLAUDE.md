# 项目概述

Mango_RVOS — 从零手写一个 RISC-V 裸机操作系统。

## 关键约定

- `out/` 及 `*.elf` / `*.bin` / `*.o` / `*.d` 等编译产物禁止提交

## 常用命令

### 开发

```bash
# 编译（默认 all，产物 out/os.elf、out/os.bin）
make

# 运行 QEMU（退出：Ctrl-A 再按 X）
make run

# 调试：QEMU -s -S 挂起等待调试器 + gdb-multiarch
make debug

# 反汇编查看
make code

# 清理编译产物
make clean
```

### 审查

```bash
# 查看当前分支状态
git status

# 查看提交历史
git log --oneline --graph

# 查看本课相对上一课的改动
git diff HEAD~1..HEAD
```

## 编码规范

### 代码格式

- C 代码赋值 `=` 两边各留一个空格；同行内多个 `=` 可对齐到最长标识符所在列（标识符最长的那行用单空格接 `=`，其余行补空格对齐到同列）
- 汇编指令助记符后用 Tab 分隔操作数；行尾注释可用 Tab 对齐到近似同列，操作数间以逗号 + 空格分隔

### 注释风格

- 注释内容来源于课程注释，一律翻译成中文：课程注释准确则直译、不准确则给修正翻译，课程没有的不另加；能一行说清不写多行
- 结构体、枚举、数组、宏等定义使用 `/* 描述 */` 格式，与缩进对齐，`/*` 和内容间留一个空格
- 函数注释采用 Doxygen 风格，必须包含 `@brief`、`@note`、`@param`、`@return` 四字段；无内容时填"无"
- 行内注释与代码同行，代码和 `//` 间留两个空格，`//` 和注释内容间留一个空格
- 段注释放在代码上方，`/*` 与当前缩进层级对齐
- 语句在准确的前提下保持精简：文件/函数头能一行说清不写多行；注释只补充无法自解释的关键信息

### 命名风格

- 函数名: `snake_case`（如 `start_kernel`、`uart_putc`、`task_create`）
- 宏 / 常量: `UPPER_CASE`（如 `UART0`、`MAXNUM_CPU`、`LSR_RX_READY`）

## Git Flow

| 分支类型 | 生命周期 | 核心职责 | 命名规范 |
|----------|----------|----------|----------|
| `main`   | 永久     | 始终可构建，阶段性打 tag | `main` |
| `feat`   | 临时     | 从 main 创建，完成后合并回 main 并删除 | `feat/功能名` |

## 参考教程

- **章节概览**：
| 目录 | 内容 |
|------|------|
| 00-bootstrap | 最小启动汇编 + 空转 C，进入 start_kernel |
| 01-helloRVOS | 串口驱动，打印 Hello |
| 02-memanagement | 物理内存管理 |
| 03-contextswitch | 任务上下文切换 |
| 04-multitask | 多任务调度 |
| 05-traps | 陷阱 / 异常处理 |
| 06-interrupts | 中断处理 |
| 07-hwtimer | 硬件定时器 |
| 08-preemptive | 抢占式调度 |
| 09-lock | 锁 |
| 10-swtimer | 软件定时器 |
| 11-syscall | 系统调用、用户态 |

- **最新源码**：`/home/mango/embedded/plct/rvos/riscv-operating-system-mooc/code/os/` — 教程对应的完整项目源码。
