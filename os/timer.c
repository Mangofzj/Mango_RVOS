#include "kernel.h"

#define TIMER_INTERVAL CLINT_TIMEBASE_FREQ	/* 中断间隔，约 1s */
#define NR_TIMERS 10

static uint32_t tick = 0;  /* 已发生的定时器中断次数 */
static struct timer timer_list[NR_TIMERS];

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
	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < NR_TIMERS; i++) {
		t->handler = NULL;  /* 以 handler 为 NULL 表示槽位空闲 */
		t->arg = NULL;
		t++;
	}

	timer_load(TIMER_INTERVAL);

	/* 使能机器态定时器中断 */
	w_mie(r_mie() | MIE_MTIE);
}

/**
 * @brief 申请一个空闲槽位并登记超时回调
 * @note 无
 * @param handler 超时后调用的回调函数
 * @param arg 传给回调函数的参数
 * @param timeout 超时时间，单位为定时器中断次数
 * @return 成功返回定时器指针，参数非法或无空闲槽位时返回 NULL
 */
struct timer *timer_create(void (*handler)(void *arg), void *arg, uint32_t timeout)
{
	/* 参数校验从简 */
	if (NULL == handler || 0 == timeout) {
		return NULL;
	}

	/* 用锁保护多个任务间共享的 timer_list */
	spin_lock();

	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < NR_TIMERS; i++) {
		if (NULL == t->handler) {
			break;
		}
		t++;
	}
	if (NULL != t->handler) {
		spin_unlock();
		return NULL;
	}

	t->handler = handler;
	t->arg = arg;
	t->timeout_tick = tick + timeout;

	spin_unlock();

	return t;
}

/**
 * @brief 删除指定的定时器
 * @note 无
 * @param timer 待删除的定时器指针
 * @return 无
 */
void timer_delete(struct timer *timer)
{
	spin_lock();

	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < NR_TIMERS; i++) {
		if (t == timer) {
			t->handler = NULL;
			t->arg = NULL;
			break;
		}
		t++;
	}

	spin_unlock();
}

/**
 * @brief 检查已超时的定时器并触发其回调
 * @note 应在中断上下文中调用，此时中断已关闭
 * @param 无
 * @return 无
 */
static inline void timer_check(void)
{
	struct timer *t = &(timer_list[0]);
	for (int i = 0; i < NR_TIMERS; i++) {
		if (NULL != t->handler) {
			if (tick >= t->timeout_tick) {
				t->handler(t->arg);

				/* 只触发一次，超时后即删除 */
				t->handler = NULL;
				t->arg = NULL;
				break;
			}
		}
		t++;
	}
}

/**
 * @brief 定时器中断服务程序，累加计数、检查超时并装载下一次中断
 * @note 由 trap.c 调用
 * @param 无
 * @return 无
 */
void timer_isr(void)
{
	tick++;
	printf("Tick: %d\n", tick);

	timer_check();

	timer_load(TIMER_INTERVAL);

	schedule();
}
