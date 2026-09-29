#include "kernel.h"

#define TASK_DELAY_COUNT 4000	/* 延时量，task_delay 内部再乘以 50000 */

struct userdata {
	int counter;
	char *str;
};

struct userdata person = {0, "JACK"};

void user_timer_handler(void *arg)
{
	if (NULL == arg)
		return;

	struct userdata *param = (struct userdata *)arg;

	param->counter++;
	printf("TIMEOUT: %s = %d\n", param->str, param->counter);
}

void user_task0(void)
{
	uart_puts("Task 0 Started!\n");

	struct timer *t1 = timer_create(user_timer_handler, &person, 3);
	if (NULL == t1) {
		uart_puts("timer_create() failed!\n");
	}
	struct timer *t2 = timer_create(user_timer_handler, &person, 5);
	if (NULL == t2) {
		uart_puts("timer_create() failed!\n");
	}
	struct timer *t3 = timer_create(user_timer_handler, &person, 7);
	if (NULL == t3) {
		uart_puts("timer_create() failed!\n");
	}

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

/* 注意不要在 user_main 中无限循环 */
void user_main(void)
{
	task_create(user_task0);
	task_create(user_task1);
}
