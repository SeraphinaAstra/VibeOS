# VibeOS Development Checklist

## Phase 0: Foundation & Boot (COMPLETED ✅)
- [x] RISC-V 64-bit architecture support
- [x] Boot loader and kernel entry point (start.S)
- [x] SBI (Supervisor Binary Interface) integration
- [x] Console I/O via SBI (putchar/getchar)
- [x] Stack setup and BSS clearing
- [x] Interrupt disabling at boot
- [x] QEMU virt machine support
- [x] Build system with Makefile
- [x] Debug flags in compilation

## Phase 1: Console & I/O Foundation (Next Priority)
- [x] Basic string functions (strlen, strcmp, strncmp, strcpy, strncpy)
- [x] Line input with backspace support (readline)
- [x] Command-line argument parsing
- [x] Basic shell with REPL
- [x] Built-in commands: help, echo, clear
- [ ] Implement printf/sprintf for formatted output
- [ ] Add printf with %d, %s, %x, %c support
- [ ] Add sprintf buffer-based formatting
- [ ] Implement proper newline handling (\n vs \r\n)
- [ ] Add puts with automatic newline
- [ ] Implement character classification functions (isdigit, isalpha, etc.)
- [ ] Add character case conversion (toupper, tolower)

## Phase 2: Keyboard & Input Handling
- [ ] Add PS/2 keyboard driver
- [ ] Implement keyboard interrupt handling
- [ ] Add key code to ASCII translation tables
- [ ] Support extended key sequences (arrow keys, function keys)
- [ ] Add input buffering with circular queue
- [ ] Implement line editing with history
- [ ] Add tab completion for commands
- [ ] Support Ctrl+C for interrupt
- [ ] Support Ctrl+D for EOF

## Phase 3: Memory Management
- [ ] Implement simple bump allocator for initial heap
- [ ] Add malloc and free functions
- [ ] Implement memory pool for fixed-size allocations
- [ ] Add memory fragmentation tracking
- [ ] Implement realloc function
- [ ] Add memory statistics/debugging (meminfo command)
- [ ] Implement page table setup
- [ ] Add virtual memory support

## Phase 4: Process Management
- [ ] Create process control block (PCB) structure
- [ ] Add process table/linked list
- [ ] Implement task switching mechanism
- [ ] Add process context switching
- [ ] Implement timer interrupt for scheduling
- [ ] Add simple round-robin scheduler
- [ ] Add process creation (fork)
- [ ] Add process termination (exit)
- [ ] Implement process state management

## Phase 5: System Calls
- [ ] Design syscall interface and numbers
- [ ] Implement syscall dispatcher
- [ ] Add syscall: exit
- [ ] Add syscall: write (to stdout)
- [ ] Add syscall: read (from stdin)
- [ ] Add syscall: open
- [ ] Add syscall: close
- [ ] Add syscall: fork
- [ ] Add syscall: exec
- [ ] Add syscall: wait
- [ ] Add syscall: brk (heap management)
- [ ] Add syscall: mmap (memory mapping)

## Phase 6: File System Foundation
- [ ] Design simple file system structure
- [ ] Add block device interface
- [ ] Add basic file metadata (inode-like structure)
- [ ] Implement file operations (open, read, write, close)
- [ ] Add directory structure and traversal
- [ ] Implement path resolution
- [ ] Add working directory (pwd) concept
- [ ] Add ls command
- [ ] Add cat command
- [ ] Add mkdir command
- [ ] Add cd command

## Phase 7: ELF Executable Loading
- [ ] Parse ELF header format
- [ ] Implement ELF program header reading
- [ ] Load ELF segments into memory
- [ ] Set up program entry point
- [ ] Add relocation support
- [ ] Implement dynamic symbol resolution
- [ ] Add exec syscall implementation
- [ ] Support command line arguments to executables

## Phase 8: Serial Port Driver
- [ ] Implement UART serial driver
- [ ] Add serial port configuration
- [ ] Add serial I/O functions
- [ ] Implement serial-based debugging console
- [ ] Add debug logging system
- [ ] Add assert/panic functions with debug output

## Phase 9: Interrupt & Exception Handling
- [ ] Set up interrupt vector table
- [ ] Implement exception handlers
- [ ] Add timer interrupt handler
- [ ] Add external interrupt handling
- [ ] Implement interrupt context preservation
- [ ] Add interrupt priority handling
- [ ] Implement interrupt masking/unmasking

## Phase 10: Graphics & Display (Optional/Advanced)
- [ ] Detect framebuffer via device tree
- [ ] Implement framebuffer driver
- [ ] Add pixel drawing primitives
- [ ] Add text rendering to framebuffer
- [ ] Implement simple font system
- [ ] Add basic graphics library
- [ ] Implement window manager basics
- [ ] Add window compositor

## Phase 11: Standard C Library (libc)
- [ ] Expand string functions (strcat, strtok, etc.)
- [ ] Add memory functions (memset, memcpy, memcmp)
- [ ] Implement math functions (basic arithmetic)
- [ ] Add time functions
- [ ] Add type conversion functions (atoi, strtol, etc.)
- [ ] Implement qsort and bsearch
- [ ] Add assert macro and implementation
- [ ] Create standard headers (stdio.h, stdlib.h, string.h, etc.)

## Phase 12: Shell Enhancements
- [ ] Add command history (up/down arrows)
- [ ] Implement pipes (|) support
- [ ] Add I/O redirection (>, <, >>)
- [ ] Implement background execution (&)
- [ ] Add job control commands (bg, fg, jobs)
- [ ] Support multiple commands per line (;)
- [ ] Add if/then/else in shell
- [ ] Add variable support in shell
- [ ] Implement for loops in shell
- [ ] Add built-in environment variables

## Phase 13: Text Editor (ed/nano-like)
- [ ] Implement simple line editor
- [ ] Add append mode
- [ ] Add delete line functionality
- [ ] Add search/replace
- [ ] Implement file loading and saving
- [ ] Add line numbering
- [ ] Add quit command
- [ ] Support multiple buffers

## Phase 14: Process Control & Signals
- [ ] Implement signal framework
- [ ] Add SIGTERM handling
- [ ] Add SIGKILL handling
- [ ] Implement signal handlers
- [ ] Add kill command
- [ ] Implement process groups
- [ ] Add job control signals
- [ ] Implement signal masks

## Phase 15: User Management & Permissions
- [ ] Add user structure and user table
- [ ] Implement login mechanism
- [ ] Add password hashing
- [ ] Create permission bits (rwx for user/group/other)
- [ ] Implement file ownership
- [ ] Add chmod command
- [ ] Add chown command
- [ ] Implement umask
- [ ] Add whoami command

## Phase 16: Build Tools (Self-Hosting)
- [ ] Add simple assembler (convert ASM to machine code)
- [ ] Implement linker for object files
- [ ] Create object file format (.o)
- [ ] Add ar tool for creating archives
- [ ] Implement make-like build system
- [ ] Add C preprocessor basics
- [ ] Build minimal C compiler targeting RISC-V
- [ ] Implement compiler self-compilation

## Phase 17: Utilities & Commands
- [ ] Add cp (copy) command
- [ ] Add mv (move) command
- [ ] Add rm (remove) command
- [ ] Add grep command
- [ ] Add wc (word count) command
- [ ] Add head/tail commands
- [ ] Add find command
- [ ] Add touch command
- [ ] Add stat command
- [ ] Add chmod/chown commands

## Phase 18: System Tools
- [ ] Add ps (process status) command
- [ ] Add top (process monitor) command
- [ ] Add free (memory status) command
- [ ] Add uname command
- [ ] Add date/time command
- [ ] Add uptime command
- [ ] Add df (disk free) command
- [ ] Add du (disk usage) command

## Phase 19: Networking (Advanced)
- [ ] Implement network stack basics
- [ ] Add Ethernet driver
- [ ] Implement IP protocol
- [ ] Add TCP/UDP support
- [ ] Implement socket interface
- [ ] Add basic network utilities (ping, netstat)
- [ ] Implement HTTP server

## Phase 20: System Optimization
- [ ] Profile kernel performance
- [ ] Optimize hot paths
- [ ] Implement caching for file I/O
- [ ] Add memory optimization
- [ ] Implement CPU-specific optimizations

---

## Progress Summary
**Completed:** 14/92
**In Progress:** 0/92
**Not Started:** 78/92

## Notes
- Current focus: Console and basic shell functionality
- Architecture: RISC-V 64-bit (rv64imac)
- Target Platform: QEMU virt machine
- Toolchain: riscv64-elf-gcc
