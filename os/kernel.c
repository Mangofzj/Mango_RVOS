#include "kernel.h"

/* 以下函数只应在此处被调用一次，因此只在此处声明，不放进 kernel.h */
extern void uart_init(void);
extern void page_init(void);
extern void trap_init(void);
extern void plic_init(void);
extern void timer_init(void);
extern void sched_init(void);
extern void user_main(void);

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");

	page_init();

	trap_init();

	plic_init();

	timer_init();

	sched_init();

	user_main();

	schedule();

	uart_puts("Should not reach here!\n");
	while (1) {};  /* 停在这里 */
}
