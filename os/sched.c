#include "kernel.h"

/* 定义在 entry.S 中 */
extern void switch_to(struct context *next);

#define TASK_STACK_SIZE 1024

/* 标准 RISC-V 调用约定要求栈指针 sp 始终 16 字节对齐 */
uint8_t __attribute__((aligned(16))) task_stack[TASK_STACK_SIZE];
struct context task_ctx;

static void w_mscratch(reg_t x)
{
	asm volatile("csrw mscratch, %0" : : "r" (x));
}

void user_task0(void);

void sched_init(void)
{
	w_mscratch(0);

	task_ctx.sp = (reg_t)&task_stack[TASK_STACK_SIZE];
	task_ctx.ra = (reg_t)user_task0;
}

void schedule(void)
{
	struct context *next = &task_ctx;
	switch_to(next);
}

void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}

void user_task0(void)
{
	uart_puts("Task 0 Created!\n");
	while (1) {
		uart_puts("Task 0 Running...\n");
		task_delay(1000);
	}
}
