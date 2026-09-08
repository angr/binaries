; A double's bit pattern assembled in rax and returned in xmm0 (MSVC x64 `or rax, rdx; movq xmm0, rax; ret`),
; next to a control that stages rax into xmm0 as scratch but returns an int in eax.
; Assemble: nasm -f elf64 -o fp_ret_via_rax_amd64.o fp_ret_via_rax_amd64.s
bits 64
section .text
global make_double, use_double, int_after_xmm, use_int

; double make_double(uint64 hi, uint64 lo): rax = hi << 32 | lo, handed over to xmm0
make_double:
    mov     rax, rdi
    shl     rax, 32
    or      rax, rsi
    movq    xmm0, rax
    ret

; double use_double(uint64 hi, uint64 lo): return make_double(hi, lo) * 2
use_double:
    sub     rsp, 8
    call    make_double
    addsd   xmm0, xmm0
    add     rsp, 8
    ret

; int int_after_xmm(uint64 x, int n): xmm0 = (double bits of x) * 2 is scratch; the return value is written after it
int_after_xmm:
    mov     rax, rdi
    movq    xmm0, rax
    addsd   xmm0, xmm0
    lea     eax, [rsi + 1]
    ret

; int use_int(uint64 x, int n): return int_after_xmm(x, n) + 2
use_int:
    sub     rsp, 8
    call    int_after_xmm
    add     eax, 2
    add     rsp, 8
    ret
