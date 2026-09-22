#ifndef __PLATFORM_H__
#define __PLATFORM_H__

#define NR_CPUS 8	/* CPU 数量上限 */

#define RAM_SIZE (128 * 1024 * 1024)	/* RAM 大小 */

#define UART0_BASE 0x10000000L	/* UART0 基地址 */

#define UART0_IRQ 10	/* UART0 中断源 */

#define PLIC_BASE 0x0c000000L	/* PLIC 基地址 */
#define PLIC_MCONTEXT_ID(hart) ((hart) * 2)	/* hart 的机器态上下文编号 */
#define PLIC_PRIORITY(irq) (PLIC_BASE + (irq) * 4)	/* 中断源优先级寄存器 */
#define PLIC_PENDING(irq) (PLIC_BASE + 0x1000 + ((irq) / 32) * 4)	/* 中断源待处理位 */
#define PLIC_MENABLE(hart, irq) (PLIC_BASE + 0x2000 + PLIC_MCONTEXT_ID(hart) * 0x80 + ((irq) / 32) * 4)	/* 中断源使能位 */
#define PLIC_MTHRESHOLD(hart) (PLIC_BASE + 0x200000 + PLIC_MCONTEXT_ID(hart) * 0x1000)	/* 优先级阈值寄存器 */
#define PLIC_MCLAIM(hart) (PLIC_BASE + 0x200004 + PLIC_MCONTEXT_ID(hart) * 0x1000)	/* 中断认领寄存器（读模式） */
#define PLIC_MCOMPLETE(hart) (PLIC_BASE + 0x200004 + PLIC_MCONTEXT_ID(hart) * 0x1000)	/* 中断完成寄存器（写模式） */

#endif /* __PLATFORM_H__ */
