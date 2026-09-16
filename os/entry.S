#define LOAD lw
#define STORE sw
#define REG_SIZE 4

# 保存通用寄存器到上下文，跳过 gp 和 tp，二者既非 caller-saved 也非 callee-saved，常用于特殊用途，RVOS 里 tp 存放 hartid，是上下文切换期间不变的全局值
.macro	reg_save base
	STORE	ra, 0*REG_SIZE(\base)
	STORE	sp, 1*REG_SIZE(\base)
	STORE	t0, 4*REG_SIZE(\base)
	STORE	t1, 5*REG_SIZE(\base)
	STORE	t2, 6*REG_SIZE(\base)
	STORE	s0, 7*REG_SIZE(\base)
	STORE	s1, 8*REG_SIZE(\base)
	STORE	a0, 9*REG_SIZE(\base)
	STORE	a1, 10*REG_SIZE(\base)
	STORE	a2, 11*REG_SIZE(\base)
	STORE	a3, 12*REG_SIZE(\base)
	STORE	a4, 13*REG_SIZE(\base)
	STORE	a5, 14*REG_SIZE(\base)
	STORE	a6, 15*REG_SIZE(\base)
	STORE	a7, 16*REG_SIZE(\base)
	STORE	s2, 17*REG_SIZE(\base)
	STORE	s3, 18*REG_SIZE(\base)
	STORE	s4, 19*REG_SIZE(\base)
	STORE	s5, 20*REG_SIZE(\base)
	STORE	s6, 21*REG_SIZE(\base)
	STORE	s7, 22*REG_SIZE(\base)
	STORE	s8, 23*REG_SIZE(\base)
	STORE	s9, 24*REG_SIZE(\base)
	STORE	s10, 25*REG_SIZE(\base)
	STORE	s11, 26*REG_SIZE(\base)
	STORE	t3, 27*REG_SIZE(\base)
	STORE	t4, 28*REG_SIZE(\base)
	STORE	t5, 29*REG_SIZE(\base)
	# t6 被用作基址寄存器，无法在此保存，需在宏外单独补存
.endm

# 恢复上下文中的通用寄存器，同样不含 gp 和 tp
.macro	reg_restore base
	LOAD	ra, 0*REG_SIZE(\base)
	LOAD	sp, 1*REG_SIZE(\base)
	LOAD	t0, 4*REG_SIZE(\base)
	LOAD	t1, 5*REG_SIZE(\base)
	LOAD	t2, 6*REG_SIZE(\base)
	LOAD	s0, 7*REG_SIZE(\base)
	LOAD	s1, 8*REG_SIZE(\base)
	LOAD	a0, 9*REG_SIZE(\base)
	LOAD	a1, 10*REG_SIZE(\base)
	LOAD	a2, 11*REG_SIZE(\base)
	LOAD	a3, 12*REG_SIZE(\base)
	LOAD	a4, 13*REG_SIZE(\base)
	LOAD	a5, 14*REG_SIZE(\base)
	LOAD	a6, 15*REG_SIZE(\base)
	LOAD	a7, 16*REG_SIZE(\base)
	LOAD	s2, 17*REG_SIZE(\base)
	LOAD	s3, 18*REG_SIZE(\base)
	LOAD	s4, 19*REG_SIZE(\base)
	LOAD	s5, 20*REG_SIZE(\base)
	LOAD	s6, 21*REG_SIZE(\base)
	LOAD	s7, 22*REG_SIZE(\base)
	LOAD	s8, 23*REG_SIZE(\base)
	LOAD	s9, 24*REG_SIZE(\base)
	LOAD	s10, 25*REG_SIZE(\base)
	LOAD	s11, 26*REG_SIZE(\base)
	LOAD	t3, 27*REG_SIZE(\base)
	LOAD	t4, 28*REG_SIZE(\base)
	LOAD	t5, 29*REG_SIZE(\base)
	LOAD	t6, 30*REG_SIZE(\base)
.endm

.text

# void switch_to(struct context *next)
# a0 存放下一个任务的上下文指针
# mscratch 存放当前任务的上下文指针
# t6 用作基址，它是 x31，恢复过程中不会被覆盖；CSR 不能作为 LOAD/STORE 的基址寄存器，所以不能用 mscratch
.global	switch_to
.balign 4
switch_to:
	# 交换 t6 与 mscratch，交换后 t6 指向当前任务的上下文
	# 首次调用时 mscratch 为 0（见 sched_init），t6 也为 0，此时无需保存
	csrrw	t6, mscratch, t6
	beqz	t6, 1f

	reg_save	t6		# 保存当前任务的上下文

	mv	t5, t6			# t5 指向当前任务的上下文
	csrr	t6, mscratch		# 取回真正的 t6
	STORE	t6, 30*REG_SIZE(t5)	# 以 t5 为基址补存 t6

1:
	csrw	mscratch, a0		# mscratch 改指向下一个任务的上下文

	mv	t6, a0			# 用 t6 指向下一个任务的上下文
	reg_restore	t6

	ret

.end
