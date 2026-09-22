#include "kernel.h"

#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ	/* 中断间隔，约 1s */

static uint32_t tick = 0;  /* 已发生的定时器中断次数 */

/**
 * @brief 装载下一次定时器中断的间隔
 * @note 无
 * @param interval 距当前时刻的间隔，单位为定时器周期数
 * @return 无
 */
static void timer_load(int interval)
{
	/* 每个 CPU 有各自独立的定时器中断源 */
	int hart = r_tp();

	*(uint64_t*)CLINT_MTIMECMP(hart) = *(uint64_t*)CLINT_MTIME + interval;
}

/**
 * @brief 初始化定时器并开启机器态定时器中断
 * @note 复位时 mtime 清零，但 mtimecmp 不复位，因此必须在此处手动初始化 mtimecmp
 * @param 无
 * @return 无
 */
void timer_init(void)
{
	timer_load(TIMER_INTERVAL);

	/* 使能机器态定时器中断 */
	w_mie(r_mie() | MIE_MTIE);

	/* 使能机器态全局中断 */
	w_mstatus(r_mstatus() | MSTATUS_MIE);
}

/**
 * @brief 定时器中断服务程序，累加计数并装载下一次中断
 * @note 由 trap.c 调用
 * @param 无
 * @return 无
 */
void timer_isr(void)
{
	tick++;
	printf("Tick: %d\n", tick);

	timer_load(TIMER_INTERVAL);
}
