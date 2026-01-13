; This is almost entirely Bran's Kernel Development Tutorial - have changed alot
[BITS 32]
global amain

extern kmain

ALIGN 4
; dont' really get why

    MULTIBOOT_PAGE_ALIGN    equ 1<<0
    MULTIBOOT_MEMORY_INFO   equ 1<<1
    MULTIBOOT_HEADER_MAGIC  equ 0x1BADB002
    MULTIBOOT_HEADER_FLAGS  equ MULTIBOOT_PAGE_ALIGN | MULTIBOOT_MEMORY_INFO
    MULTIBOOT_CHECKSUM      equ -(MULTIBOOT_HEADER_MAGIC + MULTIBOOT_HEADER_FLAGS)
section .__mbh
    align 4
MultibootHeader:
    dd MULTIBOOT_HEADER_MAGIC
    dd MULTIBOOT_HEADER_FLAGS
    dd MULTIBOOT_CHECKSUM

STACKSIZE equ 0x4000
section .text
amain:
    mov esp, stack+STACKSIZE
    push eax;
    push ebx;
    call kmain
    cli
hang:
    hlt
    jmp hang

section .bss
    align 4
stack:
    resb STACKSIZE

