; i386 [-1, 1] clamp of a double that picks the result through pointers to two stack slots (MSVC's inlined
; std::min/std::max on references): lea/lea/cmovbe on the slot addresses, then movsd xmm0, [eax]. The upper-bound
; slot is only written with 1.0 and only read through the selected pointer; it must be typed double.
;
; double clamp_ref(double x)
;
; Assemble: nasm -f elf32 -o fp_clamp_ref_i386.o fp_clamp_ref_i386.s

bits 32

section .text
global clamp_ref

clamp_ref:
    push    ebp
    mov     ebp, esp
    sub     esp, 24
    movsd   xmm1, [ebp+8]           ; x
    movsd   xmm0, [neg_one]         ; -1.0
    movsd   xmm2, [one]             ; 1.0
    comisd  xmm0, xmm1
    movsd   [ebp-16], xmm1          ; x slot
    movsd   [ebp-8], xmm2           ; upper-bound slot = 1.0
    ja      .out
    comisd  xmm1, xmm2
    lea     eax, [ebp-8]
    lea     ecx, [ebp-16]
    cmovbe  eax, ecx
    movsd   xmm0, [eax]
.out:
    movsd   [ebp-24], xmm0
    fld     qword [ebp-24]
    leave
    ret

section .rodata
neg_one: dq -1.0
one:     dq 1.0
