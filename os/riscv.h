#ifndef __RISCV_H__
#define __RISCV_H__

#include "types.h"

/* 读 tp 线程指针寄存器，RVOS 中用于存放 hart id */
static inline reg_t r_tp(void)
{
	reg_t val;
	asm volatile("mv %0, tp" : "=r"(val));
	return val;
}

/* mstatus 机器态状态寄存器的掩码 */
#define MSTATUS_MIE (1 << 3)	/* 机器态全局中断使能 */
#define MSTATUS_SIE (1 << 1)	/* 监管态全局中断使能 */
#define MSTATUS_UIE (1 << 0)	/* 用户态全局中断使能 */

/* 读 mstatus 机器态状态寄存器 */
static inline reg_t r_mstatus(void)
{
	reg_t val;
	asm volatile("csrr %0, mstatus" : "=r"(val));
	return val;
}

/* 写 mstatus 机器态状态寄存器 */
static inline void w_mstatus(reg_t val)
{
	asm volatile("csrw mstatus, %0" : : "r"(val));
}

/* mie 机器态中断使能寄存器的掩码 */
#define MIE_MSIE (1 << 3)	/* 机器态软件中断使能 */
#define MIE_MTIE (1 << 7)	/* 机器态定时器中断使能 */
#define MIE_MEIE (1 << 11)	/* 机器态外部中断使能 */

/* 读 mie 机器态中断使能寄存器 */
static inline reg_t r_mie(void)
{
	reg_t val;
	asm volatile("csrr %0, mie" : "=r"(val));
	return val;
}

/* 写 mie 机器态中断使能寄存器 */
static inline void w_mie(reg_t val)
{
	asm volatile("csrw mie, %0" : : "r"(val));
}

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
