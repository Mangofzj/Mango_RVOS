#include "kernel.h"
#include "syscall.h"

static int sys_gethartid(uint32_t *hart)
{
	printf("Get hart id! Arg0 = %p\n", hart);
	if (hart == NULL) {
		return -1;
	} else {
		*hart = r_mhartid();
		return 0;
	}
}

/**
 * @brief 处理用户态发起的系统调用，按调用号分派到对应的处理函数
 * @note 调用号取自 ctx->a7、参数取自 ctx->a0，与 RISC-V 调用约定一致；返回值写回 ctx->a0，陷阱返回时会恢复到用户态的 a0
 * @param ctx 发生系统调用的任务的上下文
 * @return 无
 */
void do_syscall(struct context *ctx)
{
	uint32_t syscall_num = ctx->a7;

	switch (syscall_num) {
	case SYS_gethartid:
		ctx->a0 = sys_gethartid((uint32_t *)(ctx->a0));
		break;
	default:
		printf("Unknown syscall! Code = %d\n", syscall_num);
		ctx->a0 = -1;
	}

	return;
}