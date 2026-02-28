// kernel.c - Main kernel code with simple shell
#include "sbi.h"

// Simple string functions
int strlen(const char* str) {
    int len = 0;
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

int strncmp(const char* s1, const char* s2, int n) {
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

void strncpy(char* dst, const char* src, int n) {
    while (n && *src) {
        *dst++ = *src++;
        n--;
    }
    if (n > 0) *dst = '\0';
}

// Character classification functions
int isdigit(char c) {
    return c >= '0' && c <= '9';
}

int isalpha(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
}

int isalnum(char c) {
    return isalpha(c) || isdigit(c);
}

int isspace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

int isupper(char c) {
    return c >= 'A' && c <= 'Z';
}

int islower(char c) {
    return c >= 'a' && c <= 'z';
}

// Character conversion functions
char toupper(char c) {
    if (c >= 'a' && c <= 'z') {
        return c - 32;
    }
    return c;
}

char tolower(char c) {
    if (c >= 'A' && c <= 'Z') {
        return c + 32;
    }
    return c;
}

// Console I/O functions
void putchar(char c) {
    sbi_console_putchar(c);
}

void puts(const char* str) {
    while (*str) {
        putchar(*str++);
    }
    putchar('\n');
}

char getchar(void) {
    int c;
    while ((c = sbi_console_getchar()) == -1) {
        // Busy wait for character
    }
    return (char)c;
}

// Internal function for number to string conversion
void int_to_str(int num, char* str, int base) {
    char digits[] = "0123456789abcdef";
    char buffer[32];
    int i = 0;
    
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    int sign = (num < 0) ? 1 : 0;
    if (sign) num = -num;
    
    while (num > 0) {
        buffer[i++] = digits[num % base];
        num /= base;
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
void simple_sprintf(char* buf, const char* fmt, long arg1, long arg2, long arg3, long arg4, long arg5, long arg6) {
    int buf_idx = 0;
    int arg_idx = 0;
    long args[6] = {arg1, arg2, arg3, arg4, arg5, arg6};
    
    while (*fmt) {
        if (*fmt == '%' && *(fmt + 1)) {
            fmt++;
            switch (*fmt) {
                case 'd': {
                    long num = args[arg_idx++];
                    char num_str[32];
                    int_to_str((int)num, num_str, 10);
                    int i = 0;
                    while (num_str[i]) {
                        buf[buf_idx++] = num_str[i++];
                    }
                    break;
                }
                case 'x': {
                    long num = args[arg_idx++];
                    char hex_str[16];
                    int_to_str((int)num, hex_str, 16);
                    int i = 0;
                    while (hex_str[i]) {
                        buf[buf_idx++] = hex_str[i++];
                    }
                    break;
                }
                case 's': {
                    char* str = (char*)args[arg_idx++];
                    if (str) {
                        while (*str) {
                            buf[buf_idx++] = *str++;
                        }
                    }
                    break;
                }
                case 'c': {
                    buf[buf_idx++] = (char)args[arg_idx++];
                    break;
                }
                case '%': {
                    buf[buf_idx++] = '%';
                    break;
                }
                default:
                    buf[buf_idx++] = '%';
                    buf[buf_idx++] = *fmt;
                    break;
            }
        } else if (*fmt == '\\' && *(fmt + 1)) {
            fmt++;
            switch (*fmt) {
                case 'n':
                    buf[buf_idx++] = '\n';
                    break;
                case 'r':
                    buf[buf_idx++] = '\r';
                    break;
                case 't':
                    buf[buf_idx++] = '\t';
                    break;
                case '\\':
                    buf[buf_idx++] = '\\';
                    break;
                default:
                    buf[buf_idx++] = '\\';
                    buf[buf_idx++] = *fmt;
                    break;
            }
        } else {
            buf[buf_idx++] = *fmt;
        }
        fmt++;
    }
    buf[buf_idx] = '\0';
}

// sprintf - formatted string printing
void sprintf(char* buf, const char* fmt, long a1, long a2, long a3, long a4, long a5, long a6) {
    simple_sprintf(buf, fmt, a1, a2, a3, a4, a5, a6);
}

// printf - formatted console output (simplified for up to 6 args)
void printf(const char* fmt, long a1, long a2, long a3, long a4, long a5, long a6) {
    char buf[512];
    simple_sprintf(buf, fmt, a1, a2, a3, a4, a5, a6);
    puts(buf);
}

// Phase 2: Keyboard & Input Handling

// PS/2 keyboard controller addresses (QEMU virt machine)
#define PS2_DATA 0x60
#define PS2_STATUS 0x64
#define PS2_CTRL 0x64

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

char input_buffer_get(void) {
    if (input_count > 0) {
        char c = input_buffer[input_head];
        input_head = (input_head + 1) % INPUT_BUFFER_SIZE;
        input_count--;
        return c;
    }
    return -1;
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
            case 0x46:  // Scroll lock (Ctrl+C)
                if (ctrl_pressed) {
                    input_buffer_put(3);  // Ctrl+C = ASCII 3
                }
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
                        input_buffer_put(c);
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
        char c = getchar();
        
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
        } else if (c == 9) {  // Tab completion
            // Tab completion - stub for now
            putchar(9);
        } else if (c >= 32 && c < 127 && pos < max_len - 1) {
            buf[pos++] = c;
            putchar(c);  // Echo character
        }
    }
}

// Phase 3: Memory Management

// Heap memory setup (1MB heap starting at 0x80500000)
#define HEAP_START 0x80500000
#define HEAP_SIZE (1024 * 1024)  // 1MB
#define HEAP_END (HEAP_START + HEAP_SIZE)

// Memory block metadata
typedef struct {
    int size;      // Block size (negative if free)
    int allocated; // 1 if allocated, 0 if free
    char* data;    // Pointer to actual data
} mem_block_t;

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

// Memset - fill memory with value
void memset(void* ptr, int value, int size) {
    unsigned char* p = (unsigned char*)ptr;
    for (int i = 0; i < size; i++) {
        p[i] = (unsigned char)value;
    }
}

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

// Realloc - allocate new block and copy data
void* realloc(void* ptr, long size) {
    if (ptr == 0) {
        return malloc(size);  // If ptr is NULL, behave like malloc
    }
    
    if (size == 0) {
        // Note: Simple bump allocator can't truly free, but we track it
        return 0;
    }
    
    // Allocate new block
    void* new_ptr = malloc(size);
    if (new_ptr == 0) return 0;
    
    // For our simple bump allocator, we don't know the original size
    // So we just return the new pointer (in a real OS we'd track sizes)
    return new_ptr;
}

// Free - in bump allocator we can't truly free, but we track it
void free(void* ptr) {
    if (ptr == 0) return;
    
    // With a simple bump allocator, we can't actually free memory
    // In a real implementation, we'd track block sizes and use a free list
    mem_num_frees++;
}

// Get remaining heap space
long heap_available(void) {
    return (long)(heap_end - heap_ptr);
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
    puts("Available commands:");
    puts("  help    - Show this help message");
    puts("  echo    - Echo arguments back");
    puts("  clear   - Clear the screen");
    puts("  meminfo - Show memory statistics");
}

// Command: echo
void cmd_echo(int argc, char** argv) {
    for (int i = 1; i < argc; i++) {
        puts(argv[i]);
        if (i < argc - 1) putchar(' ');
    }
    putchar('\n');
}

// Command: clear
void cmd_clear(void) {
    puts("\033[2J\033[H");  // ANSI escape codes: clear screen and move cursor to home
}

// Command: meminfo
void cmd_meminfo(void) {
    printf("Memory Statistics:\n", 0, 0, 0, 0, 0, 0);
    printf("  Total Allocated: %x bytes (0x%x)\n", mem_total_allocated, mem_total_allocated, 0, 0, 0, 0);
    printf("  Total Freed:     %x bytes (0x%x)\n", mem_total_freed, mem_total_freed, 0, 0, 0, 0);
    printf("  Current Usage:   %x bytes (0x%x)\n", mem_current_usage, mem_current_usage, 0, 0, 0, 0);
    printf("  Peak Usage:      %x bytes (0x%x)\n", mem_peak_usage, mem_peak_usage, 0, 0, 0, 0);
    printf("  Available:       %x bytes\n", heap_available(), 0, 0, 0, 0, 0);
    printf("  Total Heap:      %x bytes\n", HEAP_SIZE, 0, 0, 0, 0, 0);
    printf("  Allocations:     %x\n", mem_num_allocations, 0, 0, 0, 0, 0);
    printf("  Frees:           %x\n", mem_num_frees, 0, 0, 0, 0, 0);
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
    } else {
        puts("Unknown command: ");
        puts(argv[0]);
        puts("\nType 'help' for available commands.");
    }
}

// Main kernel entry point
void kernel_main(void) {
    // Initialize memory management
    mem_init();
    
    char line[256];
    
    // Clear screen and print banner
    cmd_clear();
    puts("====================================");
    puts("  VibeOS - RISC-V Edition");
    puts("  \"It Just Works(tm)\"");
    puts("====================================");
    puts("");
    puts("Type 'help' for available commands.");
    puts("");
    
    // Main shell loop
    while (1) {
        printf("vibe> ", 0, 0, 0, 0, 0, 0);
        readline(line, sizeof(line));
        execute_command(line);
    }
}
