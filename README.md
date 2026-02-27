# VibeOS - RISC-V Edition

A simple, vibecoded operating system for RISC-V architecture.

## Prerequisites

You need:
1. RISC-V GCC toolchain (`riscv64-unknown-elf-gcc`)
2. QEMU with RISC-V support (`qemu-system-riscv64`)

### Installing on Ubuntu/Debian:
```bash
sudo apt update
sudo apt install gcc-riscv64-unknown-elf qemu-system-misc
```

### Installing on macOS (with Homebrew):
```bash
brew tap riscv/riscv
brew install riscv-tools qemu
```

## Building

Just run:
```bash
make
```

This will produce `kernel.elf` - your bootable kernel!

## Running

Run in QEMU:
```bash
make run
```

Or manually:
```bash
qemu-system-riscv64 -machine virt -bios default -nographic -serial mon:stdio -kernel kernel.elf
```

## Using the OS

Once booted, you'll see a prompt:
```
vibe>
```

Available commands:
- `help` - Show available commands
- `echo <text>` - Echo text back
- `clear` - Clear the screen

To exit QEMU: Press `Ctrl-A` then `X`

## Project Structure

- `start.S` - Assembly entry point, sets up stack and calls C code
- `kernel.c` - Main kernel code with shell and commands
- `sbi.h` - OpenSBI wrapper functions for console I/O
- `linker.ld` - Linker script defining memory layout
- `Makefile` - Build system

## How It Works

1. QEMU loads OpenSBI (built-in BIOS)
2. OpenSBI initializes hardware and loads our kernel at 0x80200000
3. Kernel starts at `_start` in `start.S`
4. Sets up stack and jumps to `kernel_main()` in C
5. Shell loop reads commands and executes them

No boot sector nonsense. Just a normal ELF binary. Beautiful.
