#include "platform.h"

	# 每个 hart 的栈大小是 1024 字节
	.equ	KERNEL_STACK_SIZE, 1024

	.global	_start

	.text
_start:
	# park id 不为 0 的 hart
	csrr	t0, mhartid		# 读取当前 hart 的 id
	mv	tp, t0			# 把当前 hart 的 id 存入 tp，供后续使用
	bnez	t0, park		# 若非 0 号 hart，跳到 park 原地等待

	# 将 .bss 段的所有字节清零
	la	a0, _bss_start
	la	a1, _bss_end
	bgeu	a0, a1, 2f
1:
	sw	zero, (a0)
	addi	a0, a0, 4
	bltu	a0, a1, 1b
2:
	# 初始化栈，栈自高地址向低地址增长，故 sp 指向栈区末尾
	slli	t0, t0, 10		# hart id 左移 10 位（×1024），得本 hart 栈区偏移
	la	sp, stacks + KERNEL_STACK_SIZE	# 先把 sp 指向第一个栈的末尾
	add	sp, sp, t0		# 再把当前 hart 的 sp 移到它在栈区中的位置

	# schedule() 最终以 mret 切到第一个任务，故先备好 mstatus，不能假设其复位值为 0
	li	t0, 3 << 11 | 1 << 7	# MPP=3 保持机器态，MPIE=1 使能中断
	csrs	mstatus, t0

	j	start_kernel		# 0 号 hart 跳转到 C 代码

park:
	wfi
	j	park

	# 标准 RISC-V 调用约定要求栈指针 sp 始终 16 字节对齐
	.balign	16
stacks:
	.skip	KERNEL_STACK_SIZE * NR_CPUS	# 为所有 hart 分配栈空间

	.end				# 文件结束
