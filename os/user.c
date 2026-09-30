#include "kernel.h"

#define TASK_DELAY_COUNT 4000	/* 延时量，task_delay 内部再乘以 50000 */


void user_task0(void)
{
	uart_puts("Task 0 Started!\n");

	uint32_t hart = -1;

#ifdef CONFIG_SYSCALL

	int ret = -1;
	ret = gethartid(&hart);

	if (!ret) {
		printf("System call returned! hart id = %d\n", hart);
	} else {
		printf("gethartid() failed! Code = %d\n", ret);
	}
#endif

	while (1) {
		uart_puts("Task 0 Running...\n");
		task_delay(TASK_DELAY_COUNT);
	}
}

void user_task1(void)
{
	uart_puts("Task 1 Started!\n");
	while (1) {
		uart_puts("Task 1 Running...\n");
		task_delay(TASK_DELAY_COUNT);
	}
}

/* 注意不要在 user_init 中无限循环 */
void user_init(void)
{
	task_create(user_task0);
	task_create(user_task1);
}
