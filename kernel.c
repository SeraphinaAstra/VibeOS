// kernel.c - Main kernel code with simple shell
#include "sbi.h"

// Simple string functions
long strlen(const char* str) {
    long len = 0;
    while (str[len]) len++;
    return len;
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && (*s1 == *s2)) {
        s1++;
        s2++;
    }
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

int strncmp(const char* s1, const char* s2, long n) {
    while (n && *s1 && (*s1 == *s2)) {
        s1++;
        s2++;
        n--;
    }
    if (n == 0) return 0;
    return *(unsigned char*)s1 - *(unsigned char*)s2;
}

void strcpy(char* dst, const char* src) {
    while (*src) {
        *dst++ = *src++;
    }
    *dst = '\0';
}

void strncpy(char* dst, const char* src, long n) {
    while (n && *src) {
        *dst++ = *src++;
        n--;
    }
    if (n > 0) *dst = '\0';
}

// Character classification functions (handle unsigned char properly)
int isdigit(int c) {
    return c >= '0' && c <= '9';
}

int isalpha(int c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isalnum(int c) {
    return isalpha(c) || isdigit(c);
}

int isspace(int c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int isupper(int c) {
    return c >= 'A' && c <= 'Z';
}

int islower(int c) {
    return c >= 'a' && c <= 'z';
}

// Character conversion functions
int toupper(int c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

int tolower(int c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

// Console I/O functions
void putchar(char c) {
    sbi_console_putchar(c);
}

// puts - print string WITHOUT adding newline
void puts(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

// puts_ln - print string WITH newline
void puts_ln(const char* str) {
    puts(str);
    putchar('\n');
}

int getchar(void) {
    int c;
    while ((c = sbi_console_getchar()) == -1) {
        // Busy wait for character
    }
    return c;
}

// Memset - fill memory with value
void memset(void* ptr, int value, long size) {
    unsigned char* p = (unsigned char*)ptr;
    for (long i = 0; i < size; i++) {
        p[i] = (unsigned char)value;
    }
}

// Internal function for number to string conversion
void int_to_str(long num, char* str, int base) {
    char digits[] = "0123456789abcdef";
    char buffer[32];
    int i = 0;
    
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    // Only emit a leading '-' for base-10.
    // For hex (%x) and other bases, mimic typical printf behavior by treating the
    // value as unsigned (two's complement representation for negatives).
    int sign = (base == 10 && num < 0) ? 1 : 0;
    unsigned long unum = sign ? (unsigned long)(-(unsigned long)num) : (unsigned long)num;
    
    while (unum > 0) {
        buffer[i++] = digits[unum % (unsigned long)base];
        unum /= (unsigned long)base;
    }
    
    if (sign) str[0] = '-';
    int j = 0;
    while (i > 0) {
        str[j + sign] = buffer[--i];
        j++;
    }
    str[j + sign] = '\0';
}

// vsprintf implementation - uses variadic args directly
void simple_sprintf(char* buf, int buf_size, const char* fmt, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) {
    int buf_idx = 0;
    int arg_idx = 0;
    long args[6] = {arg1, arg2, arg3, arg4, arg5, arg6};
    
    while (*fmt && buf_idx < buf_size - 1) {
        if (*fmt == '%' && *(fmt + 1)) {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    if (arg_idx >= 6) break;
                    long num = args[arg_idx++];
                    char num_str[32];
                    int_to_str(num, num_str, 10);
                    int i = 0;
                    while (num_str[i] && buf_idx < buf_size - 1) {
                        buf[buf_idx++] = num_str[i++];
                    }
                    break;
                }
                case 'x': {
                    if (arg_idx >= 6) break;
                    long num = args[arg_idx++];
                    char hex_str[16];
                    int_to_str(num, hex_str, 16);
                    int i = 0;
                    while (hex_str[i] && buf_idx < buf_size - 1) {
                        buf[buf_idx++] = hex_str[i++];
                    }
                    break;
                }
                case 's': {
                    if (arg_idx >= 6) break;
                    char* str = (char*)args[arg_idx++];
                    if (str) {
                        while (*str && buf_idx < buf_size - 1) {
                            buf[buf_idx++] = *str++;
                        }
                    }
                    break;
                }
                case 'c': {
                    if (arg_idx >= 6) break;
                    if (buf_idx < buf_size - 1) {
                        buf[buf_idx++] = (char)args[arg_idx++];
                    }
                    break;
                }
                case '%': {
                    if (buf_idx < buf_size - 1) {
                        buf[buf_idx++] = '%';
                    }
                    break;
                }
                default:
                    if (buf_idx < buf_size - 1) buf[buf_idx++] = '%';
                    if (buf_idx < buf_size - 1) buf[buf_idx++] = *fmt;
                    break;
            }
        } else if (*fmt == '\\' && *(fmt + 1)) {
            fmt++;
            switch (*fmt) {
                case 'n':
                    if (buf_idx < buf_size - 1) buf[buf_idx++] = '\n';
                    break;
                case 'r':
                    if (buf_idx < buf_size - 1) buf[buf_idx++] = '\r';
                    break;
                case 't':
                    if (buf_idx < buf_size - 1) buf[buf_idx++] = '\t';
                    break;
                case '\\':
                    if (buf_idx < buf_size - 1) buf[buf_idx++] = '\\';
                    break;
                default:
                    if (buf_idx < buf_size - 1) buf[buf_idx++] = '\\';
                    if (buf_idx < buf_size - 1) buf[buf_idx++] = *fmt;
                    break;
            }
        } else {
            if (buf_idx < buf_size - 1) buf[buf_idx++] = *fmt;
        }
        fmt++;
    }
    buf[buf_idx] = '\0';
}


// printf - formatted console output (simplified for up to 6 args)
void printf(const char* fmt, long a1, long a2, long a3, long a4, long a5, long a6) {
    char buf[512];
    simple_sprintf(buf, sizeof(buf), fmt, a1, a2, a3, a4, a5, a6);
    puts(buf);  // Now puts() doesn't add extra newline
}

// Phase 2: Keyboard & Input Handling

// Input buffer for circular queue
#define INPUT_BUFFER_SIZE 256
char input_buffer[INPUT_BUFFER_SIZE];
int input_head = 0;
int input_tail = 0;
int input_count = 0;

// PS/2 key code to ASCII translation table (US layout)
char keycode_to_ascii[128] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 8, 9,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 13, 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\', 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Shifted key codes
char keycode_to_ascii_shift[128] = {
    0, 27, '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '_', '+', 8, 9,
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', 13, 0, 'A', 'S',
    'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '"', '~', 0, '|', 'Z', 'X', 'C', 'V',
    'B', 'N', 'M', '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, '-', 0, 0, 0, '+', 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

int shift_pressed = 0;
int ctrl_pressed = 0;
int alt_pressed = 0;

// Input buffer management
void input_buffer_put(char c) {
    if (input_count < INPUT_BUFFER_SIZE) {
        input_buffer[input_tail] = c;
        input_tail = (input_tail + 1) % INPUT_BUFFER_SIZE;
        input_count++;
    }
}

int input_buffer_get(void) {
    if (input_count > 0) {
        unsigned char c = (unsigned char)input_buffer[input_head];
        input_head = (input_head + 1) % INPUT_BUFFER_SIZE;
        input_count--;
        return (int)c;
    }
    return -1;  // EOF
}

int input_buffer_empty(void) {
    return input_count == 0;
}

// PS/2 keyboard handler
void keyboard_handler(int keycode) {
    int key_released = keycode & 0x80;
    keycode = keycode & 0x7F;
    
    if (key_released) {
        // Handle key release
        if (keycode == 0x2A || keycode == 0x36) {  // Left/Right Shift
            shift_pressed = 0;
        } else if (keycode == 0x1D) {  // Left Ctrl
            ctrl_pressed = 0;
        } else if (keycode == 0x38) {  // Left Alt
            alt_pressed = 0;
        }
    } else {
        // Handle key press
        switch (keycode) {
            case 0x2A:
            case 0x36:  // Left/Right Shift
                shift_pressed = 1;
                break;
            case 0x1D:  // Left Ctrl
                ctrl_pressed = 1;
                break;
            case 0x38:  // Left Alt
                alt_pressed = 1;
                break;
            case 0x53:  // Delete (Ctrl+D)
                if (ctrl_pressed) {
                    input_buffer_put(4);  // Ctrl+D = ASCII 4
                }
                break;
            default:
                if (keycode < 128) {
                    char c = shift_pressed ? keycode_to_ascii_shift[keycode] : keycode_to_ascii[keycode];
                    if (c != 0) {
                        if (ctrl_pressed && (c >= 'a' && c <= 'z')) {
                            input_buffer_put((char)(c - 'a' + 1));  // Ctrl+A=1, Ctrl+C=3, etc.
                        } else {
                            input_buffer_put(c);
                        }
                    }
                }
                break;
        }
    }
}

// Read a line from console with keyboard input buffering
void readline(char* buf, int max_len) {
    int pos = 0;
    while (1) {
        int c = getchar();
        
        if (c == 3) {  // Ctrl+C
            buf[pos] = '\0';
            putchar('^');
            putchar('C');
            putchar('\n');
            return;
        } else if (c == 4) {  // Ctrl+D (EOF)
            buf[pos] = '\0';
            putchar('\n');
            return;
        } else if (c == '\r' || c == '\n') {
            putchar('\n');
            buf[pos] = '\0';
            return;
        } else if (c == 127 || c == 8) {  // Backspace
            if (pos > 0) {
                pos--;
                putchar(8);
                putchar(' ');
                putchar(8);  // Erase character visually
            }
        } else if (c == 9) {  // Tab
            // Tab completion stub - just add tab to buffer if space
            if (pos < max_len - 1) {
                buf[pos++] = '\t';
                putchar(9);
            }
        } else if (c >= 32 && c < 127 && pos < max_len - 1) {
            buf[pos++] = (char)c;
            putchar((char)c);  // Echo character
        }
    }
}

// Phase 3: Memory Management

// Heap memory setup (1MB heap starting at 0x80500000)
#define HEAP_START 0x80500000
#define HEAP_SIZE (1024 * 1024)  // 1MB
#define HEAP_END (HEAP_START + HEAP_SIZE)

// Simple bump allocator with free list
static char* heap_ptr = (char*)HEAP_START;
static char* heap_end = (char*)HEAP_END;

// Memory statistics - using global variables for proper initialization
long mem_total_allocated = 0;
long mem_total_freed = 0;
long mem_current_usage = 0;
long mem_peak_usage = 0;
long mem_num_allocations = 0;
long mem_num_frees = 0;

// Initialize memory management
void mem_init(void) {
    heap_ptr = (char*)HEAP_START;
    
    // Global variables are automatically zeroed, but be explicit
    mem_total_allocated = 0;
    mem_total_freed = 0;
    mem_current_usage = 0;
    mem_peak_usage = 0;
    mem_num_allocations = 0;
    mem_num_frees = 0;
}

// Simple malloc implementation (bump allocator)
void* malloc(long size) {
    if (size <= 0 || heap_ptr + size > heap_end) {
        return 0;  // NULL - out of memory
    }
    
    void* ptr = (void*)heap_ptr;
    heap_ptr += size;
    
    mem_total_allocated += size;
    mem_current_usage += size;
    mem_num_allocations++;
    
    if (mem_current_usage > mem_peak_usage) {
        mem_peak_usage = mem_current_usage;
    }
    
    return ptr;
}

// Forward declaration of free() for use in realloc()
void free(void* ptr);

// Realloc - allocate new block and copy data
// Note: With bump allocator, we don't track original sizes, so we can't copy properly
// This is a stub that allocates new memory but doesn't copy old data
void* realloc(void* ptr, long size) {
    if (ptr == 0) {
        return malloc(size);  // If ptr is NULL, behave like malloc
    }
    
    if (size == 0) {
        // Note: bump allocator can't truly free, but we track the attempt
        free(ptr);
        return 0;
    }
    
    // Allocate new block (we don't have original size info with bump allocator)
    void* new_ptr = malloc(size);
    
    // WARNING: We cannot copy data because we don't track original block sizes
    // This is a fundamental limitation of the bump allocator
    // A real implementation would use a proper malloc with metadata
    
    return new_ptr;
}

// Free - in bump allocator we can't truly free, but we track it
// With a proper allocator, we would decrement mem_current_usage
void free(void* ptr) {
    if (ptr == 0) return;
    
    // With a simple bump allocator, we can't actually free memory
    // In a real implementation with malloc metadata, we would:
    //   1. Find the block size from metadata
    //   2. Decrement mem_current_usage by block size
    //   3. Increment mem_total_freed by block size
    //   4. Add the block back to a free list
    
    // For now, just track the attempt
    mem_num_frees++;
}

// Get remaining heap space
long heap_available(void) {
    return (long)(heap_end - heap_ptr);
}

// Phase 4: Process Management

// Process states
typedef enum {
    PROC_UNUSED = 0,
    PROC_READY = 1,
    PROC_RUNNING = 2,
    PROC_BLOCKED = 3,
    PROC_ZOMBIE = 4
} proc_state_t;

// Trap frame structure (must match assembly save/restore)
typedef struct {
    long ra, gp, tp, t0, t1, t2;
    long s0, s1;
    long a0, a1, a2, a3, a4, a5, a6, a7;
    long s2, s3, s4, s5, s6, s7, s8, s9, s10, s11;
    long t3, t4, t5, t6;
    long sepc;
    long sstatus;
} trap_frame_t;

// Process Control Block
typedef struct proc {
    int pid;                    // Process ID
    proc_state_t state;         // Current state
    trap_frame_t* trap_frame;   // Saved register state
    long* stack;                // Process stack pointer
    int priority;               // Priority level (0=highest)
    int time_slice;             // Time slice remaining
    struct proc* next;          // Next in queue
} proc_t;

// Process table
#define MAX_PROCS 16
proc_t proc_table[MAX_PROCS];
proc_t* current_proc = 0;       // Currently running process
proc_t* ready_queue = 0;        // Ready queue (linked list)
int next_pid = 1;
long ticks = 0;                 // Timer ticks counter

// Initialize process management
void proc_init(void) {
    for (int i = 0; i < MAX_PROCS; i++) {
        proc_table[i].pid = -1;
        proc_table[i].state = PROC_UNUSED;
        proc_table[i].next = 0;
    }
    ready_queue = 0;
    current_proc = 0;
    next_pid = 1;
    ticks = 0;
}

// Allocate a new process
proc_t* proc_alloc(void) {
    for (int i = 0; i < MAX_PROCS; i++) {
        if (proc_table[i].state == PROC_UNUSED) {
            proc_table[i].pid = next_pid++;
            proc_table[i].state = PROC_READY;
            proc_table[i].priority = 0;
            proc_table[i].time_slice = 10;  // 10 time slices per process
            proc_table[i].next = 0;
            
            // Allocate stack for process (4KB)
            proc_table[i].stack = (long*)malloc(4096);
            if (!proc_table[i].stack) return 0;
            
            // Allocate trap frame
            proc_table[i].trap_frame = (trap_frame_t*)malloc(sizeof(trap_frame_t));
            if (!proc_table[i].trap_frame) {
                free(proc_table[i].stack);
                return 0;
            }
            
            return &proc_table[i];
        }
    }
    return 0;  // No free process slot
}

// Free a process
void proc_free(proc_t* p) {
    if (p && p->state != PROC_UNUSED) {
        if (p->stack) free(p->stack);          // NOTE: free() is a no-op with the bump allocator (leak tracked only)
        if (p->trap_frame) free(p->trap_frame); // NOTE: free() is a no-op with the bump allocator (leak tracked only)
        p->state = PROC_UNUSED;
        p->pid = -1;
    }
}

// Add process to ready queue
void proc_enqueue(proc_t* p) {
    if (p->state == PROC_READY) {
        if (!ready_queue) {
            ready_queue = p;
            p->next = 0;
        } else {
            // Add to end of queue
            proc_t* q = ready_queue;
            while (q->next) q = q->next;
            q->next = p;
            p->next = 0;
        }
    }
}

// Get next process from ready queue (round-robin)
proc_t* proc_dequeue(void) {
    if (ready_queue) {
        proc_t* p = ready_queue;
        ready_queue = p->next;
        p->next = 0;
        return p;
    }
    return 0;
}

// Phase 9: Interrupt & Exception Handling

// Trap types
#define TRAP_TIMER 5            // Timer interrupt (bit 5 in scause)
#define TRAP_ECALL 8            // Environment call (syscall)

// Enable timer interrupt
void enable_timer(void) {
    // Enable supervisor timer interrupt in SIE
    long sie = 0x20;  // STIE (Supervisor Timer Interrupt Enable)
    asm volatile("csrs sie, %0" : : "r"(sie));
    
    // Set timer via SBI
    // For now we'll use busy-wait, proper timer setup comes later
}

// Disable interrupts
void disable_interrupts(void) {
    asm volatile("csrc sie, %0" : : "r"(~0UL));
}

// Enable interrupts
void enable_interrupts(void) {
    // Enable supervisor interrupts and user interrupts
    long sie = 0x222;  // SSIE | STIE | SEIE
    asm volatile("csrs sie, %0" : : "r"(sie));
}

// Get current program counter
long get_sepc(void) {
    long sepc;
    asm volatile("csrr %0, sepc" : "=r"(sepc));
    return sepc;
}

// Get exception cause
long get_scause(void) {
    long scause;
    asm volatile("csrr %0, scause" : "=r"(scause));
    return scause;
}

// Trap handler (called from assembly)
void handle_trap(trap_frame_t* tf) {
    long scause = get_scause();
    long is_interrupt = scause & 0x8000000000000000UL;
    long cause = scause & 0x7FFFFFFFFFFFFFFF;
    
    if (is_interrupt) {
        // Handle interrupt
        if (cause == TRAP_TIMER) {  // Timer interrupt
            ticks++;
            
            // Decrement time slice
            if (current_proc) {
                current_proc->time_slice--;
                
                // If time slice expired, do context switch
                if (current_proc->time_slice <= 0) {
                    current_proc->state = PROC_READY;
                    current_proc->time_slice = 10;
                    proc_enqueue(current_proc);
                    
                    // Get next process
                    proc_t* next = proc_dequeue();
                    if (next) {
                        next->state = PROC_RUNNING;
                        current_proc = next;
                        // In a real implementation, restore trap frame here
                    }
                }
            }
        }
    } else {
        // Handle exception
        if (cause == TRAP_ECALL) {  // Environment call (syscall)
            // Handle syscall - stub for now
            tf->sepc += 4;  // Move past ecall instruction
        } else {
            // Unhandled exception
            printf("Unhandled exception: %x\n", cause, 0, 0, 0, 0, 0);
        }
    }
}

// Parse command line into argv
int parse_args(char* line, char** argv, int max_args) {
    int argc = 0;
    char* p = line;
    
    while (*p && argc < max_args) {
        // Skip whitespace
        while (*p == ' ' || *p == '\t') p++;
        if (!*p) break;
        
        // Found start of argument
        argv[argc++] = p;
        
        // Find end of argument
        while (*p && *p != ' ' && *p != '\t') p++;
        if (*p) *p++ = '\0';
    }
    
    return argc;
}

// Command: help
void cmd_help(void) {
    puts_ln("Available commands:");
    puts_ln("  help     - Show this help message");
    puts_ln("  echo     - Echo arguments back");
    puts_ln("  clear    - Clear the screen");
    puts_ln("  meminfo  - Show memory statistics");
    puts_ln("  procs    - List active processes");
}

// Command: echo
void cmd_echo(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        // Don't use puts() which adds newline - just print the string
        for (char* p = argv[i]; *p; p++) {
            putchar(*p);
        }
        if (i < argc - 1) putchar(' ');
    }
    putchar('\n');
}

// Command: clear
void cmd_clear(void) {
    puts("\033[2J\033[H");
}

// Command: meminfo
void cmd_meminfo(void) {
    printf("Memory Statistics:\n", 0, 0, 0, 0, 0, 0);
    printf("  Total Allocated: %x bytes\n", mem_total_allocated, 0, 0, 0, 0, 0);
    printf("  Total Freed:     %x bytes\n", mem_total_freed, 0, 0, 0, 0, 0);
    printf("  Current Usage:   %x bytes\n", mem_current_usage, 0, 0, 0, 0, 0);
    printf("  Peak Usage:      %x bytes\n", mem_peak_usage, 0, 0, 0, 0, 0);
    printf("  Available:       %x bytes\n", heap_available(), 0, 0, 0, 0, 0);
    printf("  Total Heap:      %x bytes\n", HEAP_SIZE, 0, 0, 0, 0, 0);
    printf("  Allocations:     %x\n", mem_num_allocations, 0, 0, 0, 0, 0);
    printf("  Frees:           %x\n", mem_num_frees, 0, 0, 0, 0, 0);
}

// Command: procs
void cmd_procs(void) {
    printf("Process Table:\n", 0, 0, 0, 0, 0, 0);
    printf("  PID  State        Priority\n", 0, 0, 0, 0, 0, 0);
    
    for (int i = 0; i < MAX_PROCS; i++) {
        if (proc_table[i].state != PROC_UNUSED) {
            char* state_str = "";
            switch (proc_table[i].state) {
                case PROC_READY: state_str = "READY"; break;
                case PROC_RUNNING: state_str = "RUNNING"; break;
                case PROC_BLOCKED: state_str = "BLOCKED"; break;
                case PROC_ZOMBIE: state_str = "ZOMBIE"; break;
                default: state_str = "?"; break;
            }
            
            printf("  %x    %s         %d\n", proc_table[i].pid, (long)state_str, proc_table[i].priority, 0, 0, 0);
        }
    }
    printf("  Ticks: %x\n", ticks, 0, 0, 0, 0, 0);
}

// Execute a command
void execute_command(char* line) {
    char* argv[16];
    int argc = parse_args(line, argv, 16);
    
    if (argc == 0) return;
    
    if (strcmp(argv[0], "help") == 0) {
        cmd_help();
    } else if (strcmp(argv[0], "echo") == 0) {
        cmd_echo(argc, argv);
    } else if (strcmp(argv[0], "clear") == 0) {
        cmd_clear();
    } else if (strcmp(argv[0], "meminfo") == 0) {
        cmd_meminfo();
    } else if (strcmp(argv[0], "procs") == 0) {
        cmd_procs();
    } else {
        puts("Unknown command: ");
        puts(argv[0]);
        putchar('\n');
        puts_ln("Type 'help' for available commands.");
    }
}

// Main kernel entry point
void kernel_main(void) {
    // Initialize memory management
    mem_init();
    
    // Initialize process management
    proc_init();
    
    // Enable interrupts
    enable_interrupts();
    
    char line[256];
    
    // Clear screen and print banner
    cmd_clear();
    puts_ln("====================================");
    puts_ln("  VibeOS - RISC-V Edition");
    puts_ln("  \"It Just Works(tm)\"");
    puts_ln("====================================");
    puts_ln("");
    puts_ln("Type 'help' for available commands.");
    puts_ln("");
    
    // Main shell loop
    while (1) {
        printf("vibe> ", 0, 0, 0, 0, 0, 0);
        readline(line, sizeof(line));
        execute_command(line);
    }
}
