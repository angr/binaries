; roundsd/roundss with each immediate rounding mode (imm[1:0]) and with the MXCSR mode (imm[2]).
; VEX lifts them to Round(rm, x) with a constant or dynamic rm.
; build: nasm -f elf64 -o sse_round_amd64.o sse_round_amd64.s

bits 64
section .text

global round_even
global round_floor
global round_ceil
global round_trunc
global round_dyn
global round_dyn_f32

round_even:
    roundsd xmm0, xmm0, 8       ; roundeven (nearest, inexact suppressed)
    ret

round_floor:
    roundsd xmm0, xmm0, 9
    ret

round_ceil:
    roundsd xmm0, xmm0, 10
    ret

round_trunc:
    roundsd xmm0, xmm0, 11
    ret

round_dyn:
    roundsd xmm0, xmm0, 4       ; rint
    ret

round_dyn_f32:
    roundss xmm0, xmm0, 4       ; rintf
    ret
