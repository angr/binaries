; byte 1 (ah) of a computed, non-lvalue value vs. byte 1 of a real stack variable
; build: nasm -f elf32 -o byte_slice_i386.o byte_slice_i386.s
bits 32
section .text

; (char)((a0 + 24341) >> 8)
global ah_of_sum
ah_of_sum:
    mov     eax, [esp+4]
    add     eax, 24341
    movzx   eax, ah
    ret

; (char)(a0 + 24341): the lsb path
global al_of_sum
al_of_sum:
    mov     eax, [esp+4]
    add     eax, 24341
    movzx   eax, al
    ret

; *((char *)&v + 1): v is a stack slot, so its address is fine
global ah_of_slot
ah_of_slot:
    mov     eax, [esp+4]
    add     eax, 24341
    push    eax
    movzx   eax, byte [esp+1]
    add     esp, 4
    ret
