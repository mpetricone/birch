[global get_word] ; birch words, not intel words
[global set_word]
[global invalidate_tlb]

get_word:
mov edx, [esp+4]
mov dword eax, [edx]
ret


set_word:
mov edx, [esp+4]
push ebx ; for C
mov ebx, [esp+8]
mov [edx], ebx
pop ebx
xor edx, edx
ret

invalidate_tlb:
mov eax, [esp+4]
invlpg [eax]
ret
