#include "kernel.h"

void plic_init(void)
{
	int hart = r_tp();

	/* 中断源优先级，0 表示永不触发（相当于关闭该中断），1 最低、7 最高，同优先级时中断号小的优先 */
	*(uint32_t*)PLIC_PRIORITY(UART0_IRQ) = 1;

	/* 每个中断源靠 enables 寄存器中对应的位置位来使能 */
	*(uint32_t*)PLIC_MENABLE(hart, UART0_IRQ) = (1 << (UART0_IRQ % 32));

	/* 优先级不高于阈值的中断一律被屏蔽，阈值最大为 7，取 0 放行所有非 0 优先级的中断，取 7 则全部屏蔽；该阈值对 PLIC 全局生效，不区分中断源 */
	*(uint32_t*)PLIC_MTHRESHOLD(hart) = 0;

	/* 使能机器态外部中断 */
	w_mie(r_mie() | MIE_MEIE);
}

/**
 * @brief 向 PLIC 查询本次该处理哪个中断
 * @note 成功认领后，PLIC 会原子地清除对应中断源的 pending 位
 * @param 无
 * @return 优先级最高的待处理中断号，无待处理中断时返回 0
 */
int plic_claim(void)
{
	int hart = r_tp();

	int irq = *(uint32_t*)PLIC_MCLAIM(hart);

	return irq;
}

/**
 * @brief 把认领到的中断号写回 complete 寄存器，告知 PLIC 该中断已处理完毕
 * @note PLIC 不校验完成号与上次认领号是否一致；完成号对应的中断源对该上下文未使能时静默忽略
 * @param irq 从 PLIC 认领到的中断号
 * @return 无
 */
void plic_complete(int irq)
{
	int hart = r_tp();

	*(uint32_t*)PLIC_MCOMPLETE(hart) = irq;
}
