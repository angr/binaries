; amd64 functions on genuine 128-bit SSE vectors, as MSVC auto-vectorizes
; integer loops. The C must use __m128i/__m128 and _mm_* intrinsics:
;
; int lt88_mask(__m128i *a, __m128i *b, __m128i *out)
;     -- pxor/pshufd/pcmpeqd/pcmpgtd/pand/por (a 64-bit signed a < 88 per lane
;        built from 32-bit lanes), packssdw + packsswb, movd
; void add16(__m128i *p)               -- paddq with {16, 16}
; void pack_unpack(__m128i *a, __m128i *b, __m128i *out)
;                                      -- packuswb, punpckldq
; void addmul_ps(__m128 *a, __m128 *b) -- addps, mulps

bits 64

section .rodata
align 16
bias:    dd 0x80000000, 0x80000000, 0x80000000, 0x80000000
limit:   dd 0x80000058, 0x80000000, 0x80000058, 0x80000000
sixteen: dq 16, 16

section .text
global lt88_mask
global add16
global pack_unpack
global addmul_ps

lt88_mask:
    movdqa   xmm7, [rel bias]
    movdqa   xmm5, [rel limit]
    movdqa   xmm2, [rdi]
    pxor     xmm2, xmm7
    pshufd   xmm1, xmm2, 0xf5
    movdqa   xmm0, xmm7
    pcmpeqd  xmm0, xmm1
    movdqa   xmm1, xmm5
    pcmpgtd  xmm1, xmm2
    pshufd   xmm2, xmm1, 0xa0
    pand     xmm0, xmm2
    pshufd   xmm1, xmm1, 0xf5
    por      xmm0, xmm1
    movdqa   xmm3, [rsi]
    pxor     xmm3, xmm7
    pshufd   xmm1, xmm3, 0xf5
    movdqa   xmm4, xmm7
    pcmpeqd  xmm4, xmm1
    movdqa   xmm1, xmm5
    pcmpgtd  xmm1, xmm3
    pshufd   xmm3, xmm1, 0xa0
    pand     xmm4, xmm3
    pshufd   xmm1, xmm1, 0xf5
    por      xmm4, xmm1
    packssdw xmm0, xmm4
    packsswb xmm0, xmm0
    movdqa   [rdx], xmm0
    movd     eax, xmm0
    and      eax, 1
    ret

add16:
    movdqa   xmm0, [rdi]
    paddq    xmm0, [rel sixteen]
    movdqa   [rdi], xmm0
    ret

pack_unpack:
    movdqa    xmm0, [rdi]
    movdqa    xmm1, [rsi]
    packuswb  xmm0, xmm1
    punpckldq xmm0, xmm1
    movdqa    [rdx], xmm0
    ret

addmul_ps:
    movaps   xmm0, [rdi]
    movaps   xmm1, [rsi]
    addps    xmm0, xmm1
    mulps    xmm0, xmm1
    movaps   [rdi], xmm0
    ret
