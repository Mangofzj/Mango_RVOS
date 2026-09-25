OUT := out
CROSS := riscv64-unknown-elf-
CC     := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
OBJDUMP := $(CROSS)objdump
QEMU   := qemu-system-riscv32
GDB    := gdb-multiarch

CFLAGS := -march=rv32g -mabi=ilp32 -nostdlib -fno-builtin -g -Wall

# 汇编 / C 源文件：每新增文件就加进对应列表，start.S 必须保持在最前
SRCS_ASM := start.S mem.S entry.S
SRCS_C   := kernel.c uart.c printf.c page.c sched.c trap.c user.c plic.c timer.c lock.c

OBJS := $(addprefix $(OUT)/, $(SRCS_ASM:.S=.o) $(SRCS_C:.c=.o))
ELF  := $(OUT)/os.elf
BIN  := $(OUT)/os.bin

# 链接脚本：os.ld 中含 #include，需先预处理展开宏
LDSCRIPT := $(OUT)/os.ld.generated
LDFLAGS  := -T $(LDSCRIPT)

all: $(ELF)

$(ELF): $(OBJS) $(LDSCRIPT)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $(OBJS)
	$(OBJCOPY) -O binary $(ELF) $(BIN)

$(LDSCRIPT): os.ld platform.h | $(OUT)
	$(CC) -E -P -x c $(CFLAGS) os.ld -o $@

# -MMD -MP 只加在这里，让每个 .o 顺带生成 .d 记录头文件依赖；放进 CFLAGS 会连 os.ld 预处理那步也生成 .d
$(OUT)/%.o: %.S | $(OUT)
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

$(OUT)/%.o: %.c | $(OUT)
	$(CC) $(CFLAGS) -MMD -MP -c -o $@ $<

$(OUT):
	mkdir -p $@

run: all
	$(QEMU) -nographic -machine virt -bios none -smp 1 -kernel $(ELF)

debug: all
	$(QEMU) -nographic -machine virt -bios none -smp 1 -kernel $(ELF) -s -S &
	$(GDB) $(ELF) -q -x gdbinit

code: $(ELF)
	$(OBJDUMP) -S $(ELF) | less

clean:
	rm -rf $(OUT)

-include $(OBJS:.o=.d)

.PHONY: all run debug code clean
