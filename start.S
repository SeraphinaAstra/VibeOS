# start.S - RISC-V kernel entry point

.section .text.init
.global _start

_start:
    # Disable interrupts
    csrw sie, zero
    
    # Set up stack pointer
    # We'll put stack at 0x80400000 (16KB above kernel load addr)
    la sp, stack_top
    
    # Clear BSS section
    la t0, _bss_start
    la t1, _bss_end
clear_bss:
    bge t0, t1, bss_done
    sd zero, 0(t0)
    addi t0, t0, 8
    j clear_bss
    
bss_done:
    # Jump to C kernel main
    call kernel_main
    
    # If kernel_main returns, halt
halt:
    wfi
    j halt

.section .bss
.align 4
stack_bottom:
    .skip 16384  # 16KB stack
.align 16  # 16-byte alignment for RISC-V ABI compliance
stack_top:
