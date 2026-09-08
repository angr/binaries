; A float stored to a stack slot and reloaded as its raw integer bits (MSVC _ftol2 style), and an int64 round-tripped
; through a qword slot (fistp/fild).
; build: nasm -f elf32 -o float_bits_i386.o float_bits_i386.s
bits 32
section .text

; unsigned int float_sign_flip_bits(double x):  return (float_as_uint((float)x) ^ 0x80000000) + 0x7fffffff
; The xor of the sign bit feeds integer arithmetic: it is bit manipulation, not an FP negation.
global float_sign_flip_bits
float_sign_flip_bits:
    push    ebp
    mov     ebp, esp
    sub     esp, 8
    fld     qword [ebp+8]
    fstp    dword [esp]
    mov     eax, [esp]
    xor     eax, 0x80000000
    add     eax, 0x7fffffff
    leave
    ret

; unsigned int float_or_hi_sign(double x, int sel):
;   r = (long long)x;  bits = sel ? float_as_uint((float)x) : (unsigned int)(r >> 32);
;   return (int)bits < 0 ? 1 : (unsigned int)r
; edx merges the float bits of x with the high dword of the rounded value: it is an integer, not the float slot.
; The qword slot is read twice (high and low dword), so it keeps a variable instead of re-materializing (long long)x.
global float_or_hi_sign
float_or_hi_sign:
    push    ebp
    mov     ebp, esp
    sub     esp, 16
    fld     qword [ebp+8]
    fst     dword [esp+8]
    fistp   qword [esp]
    cmp     dword [ebp+16], 0
    je      .hi
    mov     edx, [esp+8]
    jmp     .test
.hi:
    mov     edx, [esp+4]
.test:
    test    edx, edx
    jns     .pos
    mov     eax, 1
    leave
    ret
.pos:
    mov     eax, [esp]
    leave
    ret
