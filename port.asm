[BITS 32]
[global inportb]
[global outportb]
; your standard inportb/outportb functions for use from within a C function
inportb:
    xor edx, edx ;see outportb for xors
    xor eax, eax
    mov dx, [esp+4]
    in al, dx
    ret
outportb:
    xor eax, eax ; i was getting values in ax cleared em just incase, since C won't need them
    xor edx, edx ; did the same for dx, it was clean
    mov dx, [esp+4]
    mov al, [esp+8]
    out dx, al
    ret
