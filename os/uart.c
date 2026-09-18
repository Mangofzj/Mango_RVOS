#include "types.h"
#include "platform.h"

#define RHR 0	/* 接收保持寄存器（读模式） */
#define THR 0	/* 发送保持寄存器（写模式） */
#define DLL 0	/* 除数锁存器低字节（写模式） */
#define DLM 1	/* 除数锁存器高字节（写模式） */
#define IER 1	/* 中断使能寄存器（写模式） */
#define FCR 2	/* FIFO 控制寄存器（写模式） */
#define ISR 2	/* 中断状态寄存器（读模式） */
#define LCR 3	/* 线路控制寄存器 */
#define MCR 4	/* 调制解调器控制寄存器 */
#define LSR 5	/* 线路状态寄存器 */
#define MSR 6	/* 调制解调器状态寄存器 */
#define SPR 7	/* 暂存寄存器 */

#define LSR_RX_READY (1 << 0)
#define LSR_TX_IDLE  (1 << 5)

#define UART_REG(reg) ((volatile uint8_t *)(UART0_BASE + reg))

#define uart_read_reg(reg) (*(UART_REG(reg)))
#define uart_write_reg(reg, val) (*(UART_REG(reg)) = (val))

void uart_init(void)
{
	uart_write_reg(IER, 0x00);  /* 关闭中断 */

	/*
	 * 设置波特率
	 * 除数锁存器与收发/中断使能寄存器共用地址，要先把 LCR 位 7（DLAB，除数锁存器访问位）置 1，之后对 DLL/DLM 的读写才指向除数寄存器
	 * 1.8432 MHz 晶振下用 38.4K 波特率，对应除数 3（0x0003），拆成两字节：DLL 存低字节 0x03，DLM 存高字节 0x00
	 * 对 QEMU-virt 来说波特率没有实际作用，这里仅走一遍流程
	 */
	uint8_t lcr = uart_read_reg(LCR);
	uart_write_reg(LCR, lcr | (1 << 7));
	uart_write_reg(DLL, 0x03);
	uart_write_reg(DLM, 0x00);

	/* 设置异步数据格式：8 位数据、1 位停止位、无校验、无 break，并关闭除数锁存 */
	lcr = 0;
	uart_write_reg(LCR, lcr | (3 << 0));
}

int uart_putc(char ch)
{
	while ((uart_read_reg(LSR) & LSR_TX_IDLE) == 0);
	return uart_write_reg(THR, ch);
}

void uart_puts(char *s)
{
	while (*s) {
		uart_putc(*s++);
	}
}