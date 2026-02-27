/* linker.ld - Linker script for RISC-V kernel */

OUTPUT_ARCH(riscv)
ENTRY(_start)

SECTIONS {
    /* QEMU virt machine loads kernel at 0x80200000 */
    . = 0x80200000;
    
    .text : {
        *(.text.init)
        *(.text .text.*)
    }
    
    .rodata : {
        *(.rodata .rodata.*)
    }
    
    .data : {
        *(.data .data.*)
    }
    
    .bss : {
        _bss_start = .;
        *(.bss .bss.*)
        *(COMMON)
        _bss_end = .;
    }
    
    /DISCARD/ : {
        *(.eh_frame)
    }
}
