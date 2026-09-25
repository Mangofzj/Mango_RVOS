#include "kernel.h"

/**
 * @brief 关闭本 hart 的全局中断，进入临界区
 * @note 只关本 hart 中断，未使用原子指令，多 hart 之间不具备互斥能力；临界区内不可主动放弃 CPU
 * @param 无
 * @return 固定返回 0
 */
int spin_lock(void)
{
	w_mstatus(r_mstatus() & ~MSTATUS_MIE);

	return 0;
}

/**
 * @brief 打开本 hart 的全局中断，退出临界区
 * @note 无条件置位 MIE，不保存加锁前的状态，只适用于不嵌套的简单场景
 * @param 无
 * @return 固定返回 0
 */
int spin_unlock(void)
{
	w_mstatus(r_mstatus() | MSTATUS_MIE);

	return 0;
}
