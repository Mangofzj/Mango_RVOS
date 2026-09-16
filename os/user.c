#include "kernel.h"

void user_task0(void)
{
	uart_puts("Task 0 Created!\n");
	while (1) {
		uart_puts("Task 0 Running...\n");
		task_delay(1000);
		task_yield();
	}
}

void user_task1(void)
{
	uart_puts("Task 1 Created!\n");
	while (1) {
		uart_puts("Task 1 Running...\n");
		task_delay(1000);
		task_yield();
	}
}

/* 注意不要在 user_main 中无限循环 */
void user_main(void)
{
	task_create(user_task0);
	task_create(user_task1);
}
