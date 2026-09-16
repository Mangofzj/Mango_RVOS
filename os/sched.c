#include "kernel.h"

/* 定义在 entry.S 中 */
extern void switch_to(struct context *next);

#define NR_TASKS 10
#define TASK_STACK_SIZE 1024

/*
 * 为任务 0 在 .bss 中静态预留任务栈空间，该空间在链接阶段确定大小和位置；不同于 start.S 中内核启动阶段使用的启动栈，任务切换后 sp 会指向这里并在其中动态保存栈帧数据
 * 标准 RISC-V 调用约定要求栈指针 sp 始终 16 字节对齐
 */
uint8_t __attribute__((aligned(16))) task_stack[NR_TASKS][TASK_STACK_SIZE];
struct context task_ctx[NR_TASKS];

/* top 标记 task_ctx 中最大可用的位置，current 指向当前任务的上下文 */
static int top = 0;
static int current = -1;

static void w_mscratch(reg_t x)
{
	asm volatile("csrw mscratch, %0" : : "r"(x));
}

void sched_init(void)
{
	w_mscratch(0);
}

/**
 * @brief 实现一个简单的循环 FIFO 调度器
 * @note 无
 * @param 无
 * @return 无
 */
void schedule(void)
{
	if (top <= 0) {
		panic("Number of task should be greater than zero!");
		return;
	}
	
	current = (current + 1) % top;
	struct context *next = &task_ctx[current];
	switch_to(next);
}

/**
 * @brief 创建一个任务
 * @note 无
 * @param task 任务入口函数
 * @return 成功返回 0，失败返回 -1
 */
int task_create(void (*task)(void))
{
	if (top < NR_TASKS) {
		task_ctx[top].sp = (reg_t)&task_stack[top][TASK_STACK_SIZE];
		task_ctx[top].ra = (reg_t)task;
		top++;
		return 0;
	} else {
		return -1;
	}
	
}

/**
 * @brief 使调用者让出 CPU，让下一个任务得以运行
 * @note 无
 * @param 无
 * @return 无
 */
void task_yield(void)
{
	schedule();
}

void task_delay(volatile int count)
{
	count *= 50000;
	while (count--);
}
