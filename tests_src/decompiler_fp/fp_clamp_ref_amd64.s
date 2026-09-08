; amd64 variant of fp_clamp_ref_i386.s: the upper-bound slot is written with a 64-bit integer immediate
; (mov rax, 0x3ff0000000000000; mov [rbp-8], rax) and read as a double through the selected pointer.
;
; void clamp_ref_imm(double x, double *out)
;
; Assemble: nasm -f elf64 -o fp_clamp_ref_amd64.o fp_clamp_ref_amd64.s

bits 64

section .text
global clamp_ref_imm

clamp_ref_imm:
    push    rbp
    mov     rbp, rsp
    movsd   [rbp-16], xmm0          ; x slot
    mov     rax, 0x3ff0000000000000
    mov     [rbp-8], rax            ; upper-bound slot = 1.0
    comisd  xmm0, [rbp-8]
    lea     rax, [rbp-8]
    lea     rcx, [rbp-16]
    cmovbe  rax, rcx
    movsd   xmm0, [rax]
    movsd   [rdi], xmm0
    pop     rbp
    ret
