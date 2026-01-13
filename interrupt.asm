[BITS 32]
[global idt_load]
[global isr_div]
[global isr_res1]
[global isr_nmi]
[global isr_breakpoint]
[global isr_overflow]
[global isr_bound]
[global isr_invopc]
[global isr_dna_nmc]
[global isr_df]
[global isr_cso]
[global isr_invtss]
[global isr_snp]
[global isr_ssf]
[global isr_gp]
[global isr_pf]
[global isr_res2]
[global isr_fpe]
[global isr_ac]
[global isr_mc]
[global isr_sse]
extern idt_pointer

idt_load:
    lidt [idt_pointer]
    ret

extern isr_chandler

isr_div:
    cli ; pretty sure this is usually done
    push byte 0 ; no error
    push byte 0 ; interrupt
    jmp isr_common
isr_res1:
    cli
    push byte 0
    push byte 1
    jmp isr_common
isr_nmi:
    cli
    push byte 0
    push byte 2
    jmp isr_common
isr_breakpoint:
    cli
    push byte 0
    push byte 3
    jmp isr_common
isr_overflow:
    cli
    push byte 0
    push byte 4
    jmp isr_common
isr_bound:
    cli
    push byte 0
    push byte 5
    jmp isr_common
isr_invopc:
    cli
    push byte 0
    push byte 6
    jmp isr_common
isr_dna_nmc:
    cli
    push byte 0
    push byte 7
    jmp isr_common
isr_df:
    cli
    push byte 8
    jmp isr_common
isr_cso:
    cli
    push byte 9
    jmp isr_common
isr_invtss:
    cli
    push byte 10
    jmp isr_common
isr_snp:
    cli
    push byte 11
    jmp isr_common
isr_ssf:
    cli
    push byte 12
    jmp isr_common
isr_gp:
    cli
    push byte 13
    jmp isr_common
isr_pf:
    cli
    push byte 14
    jmp isr_common
isr_res2:
    cli
    push byte 0
    push byte 15
    jmp isr_common
isr_fpe:
    cli
    push byte 0
    push byte 16
    jmp isr_common
isr_ac:
    cli
    push byte 17
    jmp isr_common
isr_mc:
    cli
    push byte 0
    push byte 18
    jmp isr_common
isr_sse:
    cli
    push byte 0
    push byte 19
    jmp isr_common







isr_common:
    pusha ; push all common regs to stack
    push ds
    push es
    push fs
    push gs
    mov ax,0x10 ; kernel segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov eax, esp ; the stack to eax
    push eax
    mov eax, isr_chandler
    call eax ; apparently preserves eip
    pop eax
    pop gs ; putting segment back?
    pop fs
    pop es
    pop ds
    popa ; should return common registers to pre this whole function
    add esp, 8 ; apparently to clean up the pushed error/isr
    iret ; interupt return, fixes a bunch of regs/flags

