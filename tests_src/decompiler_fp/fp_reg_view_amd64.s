; double int_sq(int x): t = (double)x stays in xmm1 and is used three times
; Assemble: nasm -f elf64 -o fp_reg_view_amd64.o fp_reg_view_amd64.s
bits 64
section .text
global int_sq
int_sq:
    cvtsi2sd xmm1, edi
    movapd  xmm0, xmm1
    mulsd   xmm0, xmm1
    addsd   xmm0, xmm1
    ret
