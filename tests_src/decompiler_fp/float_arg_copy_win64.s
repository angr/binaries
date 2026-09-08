; Microsoft x64: fmt_float(void **ctx, unsigned *out, float x)
; x arrives in xmm2 and is copied whole into the callee-saved xmm6 (movaps);
; every later use is a 32-bit lane (movss, cvtss2sd, xorps sign mask, ucomiss).
bits 64
default rel

section .text

global sgn
global fmt_float
global start

; int sgn(double x): the sign bit
sgn:
    movq    rax, xmm0
    shr     rax, 63
    ret

fmt_float:
    push    rbx
    sub     rsp, 0x30
    movaps  [rsp + 0x20], xmm6
    mov     rbx, [rcx]
    movaps  xmm6, xmm2
    movss   [rsp + 0x1c], xmm6
    cvtss2sd xmm0, xmm6
    call    sgn
    test    eax, eax
    je      .positive
    xorps   xmm6, [sign_mask]
    movss   [rsp + 0x1c], xmm6
.positive:
    mov     eax, [rsp + 0x1c]
    and     eax, 0x7f800000
    cmp     eax, 0x7f800000
    jne     .finite
    ucomiss xmm6, xmm6
    setp    al
    movzx   eax, al
    mov     [rdx], eax
    jmp     .done
.finite:
    movss   [rdx], xmm6
    mov     [rbx], rbx
.done:
    xor     eax, eax
    movaps  xmm6, [rsp + 0x20]
    add     rsp, 0x30
    pop     rbx
    ret

start:
    sub     rsp, 0x28
    lea     rcx, [ctx]
    lea     rdx, [out]
    movss   xmm2, [one]
    call    fmt_float
    add     rsp, 0x28
    ret

section .rdata align=16
sign_mask: dd 0x80000000, 0x80000000, 0x80000000, 0x80000000
one:       dd 0x3f800000

section .data align=16
ctx: dq out
out: dd 0, 0
