; loads the gdt

[BITS 32]
[global gdt_install]
extern gdt_pointer
extern gdt_pcheck
gdt_install:
    lgdt [gdt_pointer] ; just load the lgdt
    mov eax, [gdt_pointer]
    mov [gdt_pcheck], eax;
    mov ax, 0x10    ; this made sense when i remembered it was hex.
    mov ds, ax      ; this is the ring0 data
    mov ss, ax
    mov es, ax
    mov ax, 0x20    ; currently the ring3 data, store it in fs, and gs, cause why not. ring3 code should be at 0x18
    mov fs, ax
    mov gs, ax
    jmp 0x08:ret
ret:
    xchg bx, bx
    ret
 ;and that should do it.
