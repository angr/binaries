; nasm -felf32 -o straddling_stack_arg_read.o straddling_stack_arg_read.nasm && ld -m elf_i386 -o ../../tests/i386/decompiler/straddling_stack_arg_read straddling_stack_arg_read.o
; i386: a 4-byte read straddling two stack arguments ([esp+0xb] spans the end of the
; arg at +8 and the start of the arg at +12), followed by narrower read-modify-writes
; of the same stack slots, mirroring obfuscated junk code
BITS 32
SECTION .text
GLOBAL _start
GLOBAL straddle
GLOBAL callee
straddle:
    mov     ecx, [esp+8]
    movzx   ebx, word [esp+0xc]
    mov     edx, [esp+0xb]
    lea     eax, [edx-0x1b6116c6]
    push    eax
    or      word [esp+0xc], ax
    add     byte [esp+0x12], al
    call    callee
    pop     edx
    add     eax, ebx
    ret
callee:
    mov     eax, [esp+4]
    ret
_start:
    push    3
    push    2
    push    1
    call    straddle
    add     esp, 12
    mov     ebx, eax
    mov     eax, 1
    int     0x80
