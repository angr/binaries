; amd64: a 10-byte x87 extended value built on the stack as qword + word
; (strtold-style _LDOUBLE).  The sign-bit `or word [rsp+8]` sits after a join
; of paths that wrote the object with different widths/offsets; it must still
; modify the SAME object that `fld tword [rsp]` copies out.
;
; long double *x87_sign_merge(long double *out, unsigned flags, unsigned long long mant)

bits 64

section .text
global x87_sign_merge

x87_sign_merge:
    sub     rsp, 24
    mov     qword [rsp], rdx            ; mantissa (offset 0)
    mov     dword [rsp+8], 0            ; sign/exponent word (offset 8)
    mov     eax, esi
    and     eax, 7
    cmp     eax, 3
    jne     .not_nan
    mov     dword [rsp+6], 0x7fff8000   ; NaN pattern straddling offset 8
    jmp     .join
.not_nan:
    cmp     eax, 4
    jne     .join
    mov     word [rsp+8], 0x403e        ; exponent
.join:
    test    esi, 8
    je      .copy
    or      word [rsp+8], 0x8000        ; sign bit
.copy:
    fld     tword [rsp]
    mov     rax, rdi
    fstp    tword [rdi]
    add     rsp, 24
    ret
