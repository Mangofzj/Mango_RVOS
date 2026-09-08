OUT := out
CROSS := riscv64-unknown-elf-
CC     := $(CROSS)gcc
OBJCOPY := $(CROSS)objcopy
OBJDUMP := $(CROSS)objdump
QEMU   := qemu-system-riscv32
GDB    := gdb-multiarch

CFLAGS := -march=rv32g -mabi=ilp32 -nostdlib -fno-builtin -g -Wall

# 汇编 / C 源文件：每新增文件就加进对应列表，start.S 必须保持在最前
SRCS_ASM := start.S
SRCS_C   := kernel.c

OBJS := $(addprefix $(OUT)/, $(SRCS_ASM:.S=.o) $(SRCS_C:.c=.o))
ELF  := $(OUT)/os.elf
BIN  := $(OUT)/os.bin

# 代码段放 0x80000000（QEMU virt 的复位入口）
LDFLAGS := -Ttext=0x80000000

all: $(ELF)

$(ELF): $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^
	$(OBJCOPY) -O binary $(ELF) $(BIN)

$(OUT)/%.o: %.S | $(OUT)
	$(CC) $(CFLAGS) -c -o $@ $<

$(OUT)/%.o: %.c | $(OUT)
	$(CC) $(CFLAGS) -c -o $@ $<

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

.PHONY: all run debug code clean
