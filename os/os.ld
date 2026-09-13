/* os.ld —— RVOS 的链接脚本 */

#include "platform.h"

/*
 * OUTPUT_ARCH 指定输出目标机器架构。"riscv" 这个名字对 64 位和 32 位 RISC-V 通用，更细的区分由调用 gcc 时的 -march 和 -mabi 完成
 */
OUTPUT_ARCH( "riscv" )

/*
 * ENTRY 设置"入口点"，即程序中第一条被执行的指令。其参数是符号名，这里是 start.S 中定义的 _start
 */
ENTRY( _start )

/*
 * MEMORY 描述目标机器上各内存块的位置与大小，语法为：
 * MEMORY
 * {
 *     name [(attr)] : ORIGIN = origin, LENGTH = len
 *     ......
 * }
 * 每行定义一个内存区，区名在 MEMORY 内必须唯一，这里只定义了一个名为 ram 的区
 * attr 是可选的属性列表，用于指定未被显式映射的输入段能否使用该内存区，这里给了 w（可写）、x（可执行）、a（可分配），并用 ! 反转 r（只读）和 i（已初始化）
 * ORIGIN 设置内存区起始地址，这里正好放在 0x8000_0000，因为 QEMU-virt 会从这里开始执行
 * LENGTH = 128M 告诉链接器有 128MB RAM，链接器会复核以确保内容装得下
 */
MEMORY
{
	ram (wxa!ri) : ORIGIN = 0x80000000, LENGTH = RAM_SIZE
}

/*
 * SECTIONS 告诉链接器如何把输入段映射成输出段、以及把输出段放在内存何处，语法为：
 * SECTIONS
 * {
 *     sections-command
 *     sections-command
 *     ......
 * }
 * 每条 sections-command 可以是 (1) ENTRY 命令 (2) 符号赋值 (3) 输出段描述 (4) overlay 描述
 * 用 PROVIDE 定义符号，语法为 PROVIDE(symbol = expression)
 * _text_start、_text_end 等符号会被 mem.S 使用
 * 注意点号 '.' 表示当前位置，把表达式的值赋给符号即为当前的链接位置，该位置随内容增加而移动
 */
SECTIONS
{
	/* 把所有 text 段放进 .text 输出段。括号前的星号表示匹配任意目标文件的 .text 段 */
	.text : {
		PROVIDE(_text_start = .);
		*(.text .text.*)
		PROVIDE(_text_end = .);
	} > ram

	.rodata : {
		PROVIDE(_rodata_start = .);
		*(.rodata .rodata.*)
		PROVIDE(_rodata_end = .);
	} > ram

	.data : {
		/* . = ALIGN(4096) 把当前位置对齐到 4096 字节，中间填入填充字节直到落在 4KiB 边界上，因为我们的分页系统以 4096 字节为粒度 */
		. = ALIGN(4096);
		PROVIDE(_data_start = .);
		*(.sdata .sdata.*)  /* sdata 和 data 本质是同一回事，无需区分 */
		*(.data .data.*)
		PROVIDE(_data_end = .);
	} > ram

	.bss :{
		PROVIDE(_bss_start = .);
		*(.sbss .sbss.*)
		*(.bss .bss.*)
		*(COMMON)  /* 多数情况下，输入文件里的 common 符号会被放进输出文件的 .bss 段 */
		PROVIDE(_bss_end = .);
	} > ram

	PROVIDE(_memory_start = ORIGIN(ram));
	PROVIDE(_memory_end = ORIGIN(ram) + LENGTH(ram));

	PROVIDE(_heap_start = _bss_end);
	PROVIDE(_heap_size = _memory_end - _heap_start);
}
