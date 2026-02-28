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

## Phase 1: Console & I/O Foundation (COMPLETED ✅)
- [x] Basic string functions (strlen, strcmp, strncmp, strcpy, strncpy)
- [x] Line input with backspace support (readline)
- [x] Command-line argument parsing
- [x] Basic shell with REPL
- [x] Built-in commands: help, echo, clear
- [x] Implement printf/sprintf for formatted output
- [x] Add printf with %d, %s, %x, %c support
- [x] Add sprintf buffer-based formatting
- [x] Implement proper newline handling (\n vs \r\n)
- [x] Add puts with automatic newline
- [x] Implement character classification functions (isdigit, isalpha, etc.)
- [x] Add character case conversion (toupper, tolower)

## Phase 2: Keyboard & Input Handling (COMPLETED ✅)
- [x] Add PS/2 keyboard driver
- [x] Implement keyboard interrupt handling
- [x] Add key code to ASCII translation tables
- [x] Support extended key sequences (arrow keys, function keys)
- [x] Add input buffering with circular queue
- [x] Implement line editing with history
- [x] Add tab completion for commands
- [x] Support Ctrl+C for interrupt
- [x] Support Ctrl+D for EOF

## Phase 3: Memory Management (COMPLETED ✅)
- [x] Implement simple bump allocator for initial heap
- [x] Add malloc and free functions
- [x] Implement memory pool for fixed-size allocations
- [x] Add memory fragmentation tracking
- [x] Implement realloc function
- [x] Add memory statistics/debugging (meminfo command)
- [x] Implement page table setup
- [x] Add virtual memory support

## Phase 4: Process Management
- [ ] Create process control block (PCB) structure with state/priority/context
- [ ] Add process table (linked list or array-based)
- [ ] Implement task context save/restore (register state)
- [ ] Implement context switching at privilege boundary
- [ ] Implement timer interrupt handler (mtimer for RISC-V)
- [ ] Add simple round-robin scheduler with time slices
- [ ] Implement process creation (fork with copy-on-write concept)
- [ ] Implement process termination with cleanup
- [ ] Add process state transitions (ready/running/blocked/zombie)
- [ ] Implement wait/reap for process synchronization

## Phase 5: System Calls & Traps
- [ ] Set up ecall trap handler in S-mode
- [ ] Design syscall numbers (SYS_exit, SYS_write, SYS_read, etc.)
- [ ] Implement syscall dispatcher/multiplexer
- [ ] Add syscall: exit (process termination)
- [ ] Add syscall: write (stdout output)
- [ ] Add syscall: read (stdin input)
- [ ] Add syscall: open (file operations)
- [ ] Add syscall: close (file cleanup)
- [ ] Add syscall: fork (process creation)
- [ ] Add syscall: exec (program replacement)
- [ ] Add syscall: wait (process synchronization)
- [ ] Add syscall: brk (heap expansion)
- [ ] Add syscall: stat/fstat (file metadata)
- [ ] Add syscall: lseek (file seeking)

## Phase 6: FAT16 File System
- [ ] Design block device abstraction layer
- [ ] Add FAT16 boot sector parsing (BPB - BIOS Parameter Block)
- [ ] Implement File Allocation Table (FAT) reading
- [ ] Parse root directory entries
- [ ] Implement directory entry structure (DOS 8.3 format)
- [ ] Add file opening by path traversal
- [ ] Implement file reading with cluster chain following
- [ ] Implement file writing with cluster allocation
- [ ] Add directory listing (ls command)
- [ ] Implement mkdir with new directory creation
- [ ] Implement file deletion (free clusters in FAT)
- [ ] Add working directory tracking (cd command)
- [ ] Implement path resolution (absolute/relative)
- [ ] Add pwd command
- [ ] Support long filenames (LFN) extension
- [ ] Implement file truncation

## Phase 7: ELF Executable Loading
- [ ] Parse ELF header (magic, architecture, entry point)
- [ ] Validate ELF file format
- [ ] Implement ELF program header reading and validation
- [ ] Load ELF segments (PT_LOAD) into memory at correct addresses
- [ ] Set up program entry point and stack
- [ ] Implement ELF section header parsing
- [ ] Add symbol table reading for relocation
- [ ] Implement relocation entries (R_RISCV_* types)
- [ ] Handle dynamic symbol resolution for relocations
- [ ] Add exec syscall to replace process image
- [ ] Support command-line argument passing to executables
- [ ] Implement environment variable passing
- [ ] Add shared library loading (basic)

## Phase 8: Serial Port Driver & Debugging
- [ ] Implement UART (16550 compatible) driver for QEMU
- [ ] Add serial port initialization and configuration
- [ ] Implement serial_putchar and serial_getchar
- [ ] Add printf redirect to serial output
- [ ] Implement debug logging framework
- [ ] Add panic/abort functions with stack trace
- [ ] Implement assert macro with debug info
- [ ] Add performance profiling timer
- [ ] Add debug breakpoint support

## Phase 9: Interrupt & Exception Handling
- [ ] Set up RISC-V interrupt/exception vector in stvec
- [ ] Implement trap handler entry point in assembly
- [ ] Implement context switching in trap handler
- [ ] Add timer interrupt handler (mtimer via SBI)
- [ ] Implement external interrupt dispatcher
- [ ] Add page fault handler (invalid memory access)
- [ ] Implement illegal instruction handler
- [ ] Add privileged instruction exception handler
- [ ] Implement interrupt context preservation (all registers)
- [ ] Add interrupt priority/nesting support
- [ ] Implement interrupt masking (SIE register)
- [ ] Add software interrupt support for IPI

## Phase 10: Virtual Memory & Paging
- [ ] Set up page table structures (page tables for S-mode)
- [ ] Implement page table entry (PTE) management
- [ ] Add virtual-to-physical address translation
- [ ] Implement page allocation and deallocation
- [ ] Add demand paging (lazy allocation)
- [ ] Implement copy-on-write for fork
- [ ] Add page fault handling with swapping
- [ ] Implement TLB (translation lookaside buffer) invalidation
- [ ] Add memory protection flags (execute/write/read)
- [ ] Implement address space isolation between processes

## Phase 11: Graphics & Display (Optional/Advanced)
- [ ] Detect framebuffer via device tree
- [ ] Implement framebuffer driver
- [ ] Add pixel drawing primitives (point, line, rect)
- [ ] Add text rendering to framebuffer
- [ ] Implement simple bitmap font system
- [ ] Add basic graphics library (2D drawing)
- [ ] Implement window manager basics
- [ ] Add window compositor with z-ordering
- [ ] Add mouse/input handling to windows
- [ ] Implement screen refresh management

## Phase 12: Standard C Library (libc)
- [ ] Expand string functions (strcat, strtok, strstr, etc.)
- [ ] Add memory functions (memset, memcpy, memcmp, memmove)
- [ ] Implement math functions (abs, min, max, sqrt, pow)
- [ ] Add time functions (time, ctime, gmtime)
- [ ] Add type conversion functions (atoi, atol, strtol, strtof)
- [ ] Implement qsort and bsearch
- [ ] Add assert macro and implementation
- [ ] Create standard headers (stdio.h, stdlib.h, string.h, math.h, etc.)
- [ ] Add printf family (fprintf, vprintf, snprintf)
- [ ] Implement character I/O (getc, putc, fgetc, fputc)

## Phase 13: Shell Enhancements
- [ ] Add command history (up/down arrows)
- [ ] Implement pipes (|) with process communication
- [ ] Add I/O redirection (>, <, >>)
- [ ] Implement background execution (&)
- [ ] Add job control commands (bg, fg, jobs, disown)
- [ ] Support multiple commands per line (;)
- [ ] Add if/then/else conditionals in shell
- [ ] Add variable support and expansion ($VAR)
- [ ] Implement for loops in shell
- [ ] Add shell functions and procedures
- [ ] Implement while/until loops
- [ ] Add command substitution ($(...))

## Phase 14: Text Editor (nano-like)
- [ ] Implement line-based editing with buffer
- [ ] Add append mode for text insertion
- [ ] Add delete/cut functionality
- [ ] Add search (Ctrl+F) and replace (Ctrl+H)
- [ ] Implement file loading and saving
- [ ] Add line numbering display
- [ ] Add quit command with save prompt
- [ ] Support multiple buffers/tabs
- [ ] Add undo/redo functionality
- [ ] Implement syntax highlighting (basic)
- [ ] Add goto line functionality
- [ ] Add copy/paste buffers

## Phase 15: Process Control & Signals
- [ ] Implement signal framework and signal table
- [ ] Add signal handlers and handler registration
- [ ] Implement SIGTERM (termination) handling
- [ ] Implement SIGKILL (forced termination) handling
- [ ] Add SIGSTOP/SIGCONT for process suspension
- [ ] Implement kill command for sending signals
- [ ] Add process groups for job control
- [ ] Implement signal masks (blocking/unblocking)
- [ ] Add signal delivery and context
- [ ] Implement signal-safe functions list

## Phase 16: User Management & Permissions
- [ ] Add user structure and global user table
- [ ] Implement login mechanism with authentication
- [ ] Add password hashing (simple hash or bcrypt-like)
- [ ] Create permission bits (rwx for user/group/other)
- [ ] Implement file ownership (uid/gid)
- [ ] Add chmod command for permission changes
- [ ] Add chown command for ownership changes
- [ ] Implement umask for default permissions
- [ ] Add whoami and id commands
- [ ] Implement su (switch user) command
- [ ] Add group management basics

## Phase 17: Build Tools (Self-Hosting)
- [ ] Add simple assembler (convert ASM to machine code)
- [ ] Implement linker for object files (.o)
- [ ] Create ELF object file format support
- [ ] Add ar tool for creating archives
- [ ] Implement make-like build system (vmake)
- [ ] Add C preprocessor basics (#define, #include, #ifdef)
- [ ] Build minimal C compiler for RISC-V (tcc-like)
- [ ] Implement compiler self-compilation
- [ ] Add symbol resolution and linking
- [ ] Implement optimization passes (basic)

## Phase 18: Utilities & Commands
- [ ] Add cp (copy) command with directory recursion
- [ ] Add mv (move/rename) command with cross-filesystem support
- [ ] Add rm (remove) command with recursive deletion
- [ ] Add grep command with pattern matching
- [ ] Add wc (word count) command with line/word/byte counts
- [ ] Add head/tail commands with line limiting
- [ ] Add find command with pattern searching
- [ ] Add touch command for file creation
- [ ] Add stat command for file information
- [ ] Add hexdump command for binary viewing
- [ ] Add od (octal dump) command
- [ ] Add ln (symlink) command

## Phase 19: System Tools & Monitoring
- [ ] Add ps (process status) command with detailed info
- [ ] Add top (process monitor) with CPU/memory sorting
- [ ] Add free (memory status) command
- [ ] Add uname command for system information
- [ ] Add date/time command with formatting
- [ ] Add uptime command showing system runtime
- [ ] Add df (disk free) command with filesystem info
- [ ] Add du (disk usage) command for directory sizes
- [ ] Add dmesg for kernel message logging
- [ ] Add strace for syscall tracing
- [ ] Add env command for environment variables

## Phase 20: Networking (Advanced)
- [ ] Implement network stack architecture (OSI layers)
- [ ] Add Ethernet driver for QEMU virtio-net
- [ ] Implement ARP (Address Resolution Protocol)
- [ ] Implement IPv4 protocol stack
- [ ] Add ICMP (ping) support
- [ ] Implement UDP protocol
- [ ] Implement TCP protocol with state machine
- [ ] Add socket interface (BSD sockets)
- [ ] Implement inet_aton/inet_ntoa for IP parsing
- [ ] Add DNS client (basic lookup)
- [ ] Add ping and netstat utilities
- [ ] Implement basic HTTP server

## Phase 21: System Optimization & Profiling
- [ ] Implement performance profiling framework
- [ ] Add cycle counter for benchmarking
- [ ] Profile kernel hot paths
- [ ] Implement caching for file I/O
- [ ] Add buffer caching system
- [ ] Implement page cache
- [ ] Optimize context switching
- [ ] Add CPU affinity (per-core scheduling)
- [ ] Implement memory defragmentation
- [ ] Add CPU-specific optimizations

## Phase 22: Device & Driver Framework
- [ ] Create generic device abstraction layer
- [ ] Implement device tree parsing from bootloader
- [ ] Add device driver registration system
- [ ] Implement interrupt request (IRQ) management
- [ ] Add DMA support framework
- [ ] Create character device interface
- [ ] Create block device interface
- [ ] Implement device-to-driver binding
- [ ] Add hotplug detection framework
- [ ] Implement power management basics

---

## Progress Summary
**Completed:** 39/162
**In Progress:** 0/162
**Not Started:** 123/162

## Update Notes
- **Phase 6 Updated:** Now specifically FAT16 filesystem with detailed cluster/FAT operations
- **Phase 4-5:** Added more technical details for process management and syscalls
- **Phase 7-9:** Expanded with RISC-V specific details and relocation info
- **Phases 13-22:** Significantly enhanced with specific requirements and commands
- **New Phase 10:** Virtual memory & paging moved to dedicated phase
- **New Phases 20-22:** Networking, optimization, and device framework

## Implementation Notes
- FAT16 chosen for simplicity while being practical
- RISC-V S-mode (Supervisor mode) for kernel execution
- Interrupt handling uses stvec (trap vector)
- Process scheduling via timer interrupts (CLINT mtimer)
- ELF loading required for exec syscall and user programs
- Networking stack optional for embedded use case
