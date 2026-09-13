#include "os.h"

/* 以下函数只应在此处被调用一次，因此只在此处声明，不放进 os.h */
extern void uart_init(void);
extern void page_init(void);

void start_kernel(void)
{
	uart_init();
	uart_puts("Hello, RVOS!\n");

	page_init();

	while (1) {};  /* 停在这里 */
}
