# Makefile for VibeOS

# Toolchain - Updated for Arch Linux naming conventions
PREFIX = riscv64-elf-
CC      = $(PREFIX)gcc
AS      = $(PREFIX)as
LD      = $(PREFIX)ld
OBJCOPY = $(PREFIX)objcopy
OBJDUMP = $(PREFIX)objdump

# Flags
# Added -g for debugging and -I. to ensure headers in the current dir are found
CFLAGS = -march=rv64imac_zicsr -mabi=lp64 -mcmodel=medany \
         -nostdlib -nostartfiles -ffreestanding -O2 -Wall -g -I.
LDFLAGS = -T linker.ld

# Files
OBJS = start.o kernel.o

# Default target
all: kernel.elf

kernel.elf: $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

# Generic rule for assembly files
%.o: %.S
	$(CC) $(CFLAGS) -c $< -o $@

# Generic rule for C files
%.o: %.c sbi.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f *.o kernel.elf

# Added 'touch' to the run command to prevent that timestamp warning
run: kernel.elf
	@touch Makefile start.S kernel.c 2>/dev/null || true
	qemu-system-riscv64 -machine virt -bios default -nographic -serial mon:stdio -kernel kernel.elf

.PHONY: all clean run