# start.S - RISC-V kernel entry point

.section .text.init
.global _start

_start:
    # Disable interrupts during boot
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
    # Set up trap vector (stvec) to point to trap handler
    la t0, trap_handler
    csrw stvec, t0
    
    # Jump to C kernel main
    call kernel_main
    
    # If kernel_main returns, halt
halt:
    wfi
    j halt

# Trap handler - entry point for all exceptions and interrupts
.align 4
.global trap_handler
trap_handler:
    # Save all registers (context switch)
    # Create trap frame on stack
    addi sp, sp, -272  # 34 registers * 8 bytes
    
    # Save general purpose registers
    sd ra, 0(sp)
    sd gp, 8(sp)
    sd tp, 16(sp)
    sd t0, 24(sp)
    sd t1, 32(sp)
    sd t2, 40(sp)
    sd s0, 48(sp)
    sd s1, 56(sp)
    sd a0, 64(sp)
    sd a1, 72(sp)
    sd a2, 80(sp)
    sd a3, 88(sp)
    sd a4, 96(sp)
    sd a5, 104(sp)
    sd a6, 112(sp)
    sd a7, 120(sp)
    sd s2, 128(sp)
    sd s3, 136(sp)
    sd s4, 144(sp)
    sd s5, 152(sp)
    sd s6, 160(sp)
    sd s7, 168(sp)
    sd s8, 176(sp)
    sd s9, 184(sp)
    sd s10, 192(sp)
    sd s11, 200(sp)
    sd t3, 208(sp)
    sd t4, 216(sp)
    sd t5, 224(sp)
    sd t6, 232(sp)
    
    # Save program counter (return address)
    csrr t0, sepc
    sd t0, 240(sp)
    
    # Save status register
    csrr t0, sstatus
    sd t0, 248(sp)
    
    # Call C trap handler with trap frame pointer
    mv a0, sp
    call handle_trap
    
    # Restore registers and return
    ld ra, 0(sp)
    ld gp, 8(sp)
    ld tp, 16(sp)
    ld t0, 24(sp)
    ld t1, 32(sp)
    ld t2, 40(sp)
    ld s0, 48(sp)
    ld s1, 56(sp)
    ld a0, 64(sp)
    ld a1, 72(sp)
    ld a2, 80(sp)
    ld a3, 88(sp)
    ld a4, 96(sp)
    ld a5, 104(sp)
    ld a6, 112(sp)
    ld a7, 120(sp)
    ld s2, 128(sp)
    ld s3, 136(sp)
    ld s4, 144(sp)
    ld s5, 152(sp)
    ld s6, 160(sp)
    ld s7, 168(sp)
    ld s8, 176(sp)
    ld s9, 184(sp)
    ld s10, 192(sp)
    ld s11, 200(sp)
    ld t3, 208(sp)
    ld t4, 216(sp)
    ld t5, 224(sp)
    ld t6, 232(sp)
    
    # Restore program counter
    ld t0, 240(sp)
    csrw sepc, t0
    
    # Restore status register
    ld t0, 248(sp)
    csrw sstatus, t0
    
    addi sp, sp, 272
    sret

.section .bss
.align 4
stack_bottom:
    .skip 16384  # 16KB stack
.align 16  # 16-byte alignment for RISC-V ABI compliance
stack_top:
