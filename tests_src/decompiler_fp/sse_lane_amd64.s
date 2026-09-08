; amd64 functions that apply lane-wise SSE ops to scalar doubles, as MSVC and
; libm do for bit tricks on IEEE-754 values.  Only lane 0 of each 128-bit
; result is ever read, so the decompiler must lower the vector op to the
; scalar op on lane 0:
;
; int exponent_bits(double x)          -- psrlq 52 + pextrw   -> (bits >> 52) & 0x7ff
; int is_one(double x)                 -- cmpeqsd + pextrw    -> x == 1.0
; long long sub_lane0(double x, long long k) -- psubq + movq  -> bits - k
; double mulpd_lane0(double a, double b) -- unpcklpd + mulpd  -> a * b

bits 64

section .text
global exponent_bits
global is_one
global sub_lane0
global mulpd_lane0

exponent_bits:
    psrlq    xmm0, 52
    pextrw   eax, xmm0, 0
    and      eax, 0x7ff
    ret

is_one:
    mov      rax, 0x3ff0000000000000
    movq     xmm1, rax
    cmpeqsd  xmm1, xmm0
    pextrw   eax, xmm1, 0
    and      eax, 1
    ret

sub_lane0:
    movq     xmm1, rdi
    psubq    xmm0, xmm1
    movq     rax, xmm0
    ret

mulpd_lane0:
    unpcklpd xmm1, xmm1
    mulpd    xmm0, xmm1
    ret
