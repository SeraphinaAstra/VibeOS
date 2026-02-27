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

// Console I/O functions
void putchar(char c) {
    sbi_console_putchar(c);
}

void puts(const char* str) {
    while (*str) {
        putchar(*str++);
    }
}

char getchar(void) {
    int c;
    while ((c = sbi_console_getchar()) == -1) {
        // Busy wait for character
    }
    return (char)c;
}

// Read a line from console
void readline(char* buf, int max_len) {
    int pos = 0;
    while (1) {
        char c = getchar();
        
        if (c == '\r' || c == '\n') {
            putchar('\n');
            buf[pos] = '\0';
            return;
        } else if (c == 127 || c == 8) {  // Backspace
            if (pos > 0) {
                pos--;
                puts("\b \b");  // Erase character visually
            }
        } else if (c >= 32 && c < 127 && pos < max_len - 1) {
            buf[pos++] = c;
            putchar(c);  // Echo character
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
    puts("Available commands:\n");
    puts("  help  - Show this help message\n");
    puts("  echo  - Echo arguments back\n");
    puts("  clear - Clear the screen\n");
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
    } else {
        puts("Unknown command: ");
        puts(argv[0]);
        puts("\nType 'help' for available commands.\n");
    }
}

// Main kernel entry point
void kernel_main(void) {
    char line[256];
    
    // Clear screen and print banner
    cmd_clear();
    puts("====================================\n");
    puts("  VibeOS - RISC-V Edition\n");
    puts("  \"It Just Works(tm)\"\n");
    puts("====================================\n\n");
    puts("Type 'help' for available commands.\n\n");
    
    // Main shell loop
    while (1) {
        puts("vibe> ");
        readline(line, sizeof(line));
        execute_command(line);
    }
}
