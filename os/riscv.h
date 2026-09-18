#ifndef __RISCV_H__
#define __RISCV_H__

/* 写 mtvec 机器态陷阱向量基址寄存器 */
static inline void w_mtvec(reg_t val)
{
	asm volatile("csrw mtvec, %0" : : "r"(val));
}

/* 写 mscratch 机器态暂存寄存器，供早期陷阱处理程序使用 */
static inline void w_mscratch(reg_t val)
{
	asm volatile("csrw mscratch, %0" : : "r"(val));
}

/* mcause 机器态陷阱原因寄存器的掩码 */
#define MCAUSE_INTERRUPT_MASK (reg_t)0x80000000
#define MCAUSE_ECODE_MASK (reg_t)0x7FFFFFFF

/* 机器态中断码 */
#define IRQ_M_SOFTWARE 3
#define IRQ_M_TIMER 7
#define IRQ_M_EXTERNAL 11

#endif /* __RISCV_H__ */
