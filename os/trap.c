#include "kernel.h"

/* 定义在 entry.S 中 */
extern void trap_vector(void);
extern void uart_isr(void);
extern void timer_isr(void);

/**
 * @brief 设置机器态陷阱向量基址
 * @note 无
 * @param 无
 * @return 无
 */
void trap_init(void)
{
	w_mtvec((reg_t)trap_vector);
}

static void external_interrupt_handler(void)
{
	int irq = plic_claim();

	if (irq == UART0_IRQ) {
		uart_isr();
	} else if (irq) {
		printf("Unexpected interrupt! IRQ = %d\n", irq);
	}

	if (irq) {
		plic_complete(irq);
	}
}

/**
 * @brief 陷阱处理函数，按 mcause 区分中断与异常并分派
 * @note 无
 * @param mepc 陷阱发生时的程序计数器
 * @param mcause 陷阱原因，最高位为 1 表示中断，其余位为编码
 * @param ctx 当前任务的上下文
 * @return 陷阱返回后继续执行的地址
 */
reg_t trap_handler(reg_t mepc, reg_t mcause, struct context *ctx)
{
	reg_t return_pc = mepc;
	reg_t cause_code = mcause & MCAUSE_ECODE_MASK;

	if (mcause & MCAUSE_INTERRUPT_MASK) {
		/* 异步陷阱：中断 */
		switch (cause_code) {
		case IRQ_M_SOFTWARE:
			uart_puts("Software interrupt!\n");

			int hart = r_tp();  /* 清除 mip 中的 MSIP 位以应答软件中断 */

			*(uint32_t*)CLINT_MSIP(hart) = 0;

			schedule();
			break;
		case IRQ_M_TIMER:
			uart_puts("Timer interrupt!\n");
			timer_isr();
			break;
		case IRQ_M_EXTERNAL:
			uart_puts("External interrupt!\n");
			external_interrupt_handler();
			break;
		default:
			printf("Unknown interrupt! Code = %d\n", cause_code);
			break;
		}
	} else {
		/* 同步陷阱：异常 */
		printf("Synchronous exception! Code = %d\n", cause_code);
		switch (cause_code) {
		case ECODE_ECALL_U:
			uart_puts("System call from U-mode!\n");
			do_syscall(ctx);
			return_pc += 4;
			break;
		default:
			panic("Unhandled synchronous exception!");
		}
	}

	return return_pc;
}

/**
 * @brief 触发一次同步异常，用于验证陷阱处理流程
 * @note 无
 * @param 无
 * @return 无
 */
void trap_test(void)
{
	/* 同步异常码 7：存储/AMO 访问错误 */
	*(int *)0x00000000 = 100;

	/* 同步异常码 5：读取访问错误 */
	// int a = *(int *)0x00000000;

	uart_puts("Returned from trap!\n");
}
