#include "kernel.h"

/* 以下函数只应在此处被调用一次，因此只在此处声明，不放进 kernel.h */
extern void uart_init(void);
extern void page_init(void);
extern void sched_init(void);
extern void schedule(void);

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");

	page_init();

	sched_init();

	schedule();

	uart_puts("Should not reach here!\n");

	while (1) {};  /* 停在这里 */
}
