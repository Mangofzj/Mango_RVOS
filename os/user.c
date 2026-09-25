#include "kernel.h"

#define TASK_DELAY_COUNT 4000	/* 延时量，task_delay 内部再乘以 50000 */

#define USE_LOCK	/* 打开临界区加锁 */

void user_task0(void)
{
	uart_puts("Task 0 Started!\n");
	while (1) {
#ifdef USE_LOCK
		/* 临界区：整轮 5 次打印必须连续完成，不被定时器中断打断 */
		spin_lock();
#endif
		uart_puts("Task 0 Loop Begin!\n");
		for (int i = 0; i < 5; i++) {
			uart_puts("Task 0 Running...\n");
			task_delay(TASK_DELAY_COUNT);
		}
		uart_puts("Task 0 Loop End!\n");
#ifdef USE_LOCK
		spin_unlock();
#endif
	}
}

void user_task1(void)
{
	uart_puts("Task 1 Started!\n");
	while (1) {
		uart_puts("Task 1 Loop Begin!\n");
		for (int i = 0; i < 5; i++) {
			uart_puts("Task 1 Running...\n");
			task_delay(TASK_DELAY_COUNT);
		}
		uart_puts("Task 1 Loop End!\n");
	}
}

/* 注意不要在 user_main 中无限循环 */
void user_main(void)
{
	task_create(user_task0);
	task_create(user_task1);
}
