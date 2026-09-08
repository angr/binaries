; amd64 functions that use lane-wise SSE conversions (cvtdq2ps) on a scalar
; that was widened into lane 0 with movd.  MSVC emits this for an inlined
; floorf.  The decompiler must lower these to scalar (float) casts.
;
; float int_to_float(int i)       -- movd + cvtdq2ps -> (float)i
; float floorf_idiom(float x)     -- cvttss2si / movd / cvtdq2ps / ucomiss

bits 64

section .text
global int_to_float
global floorf_idiom

int_to_float:
    movd     xmm0, edi
    cvtdq2ps xmm0, xmm0
    ret

floorf_idiom:
    cvttss2si eax, xmm0
    movd     xmm1, eax
    cvtdq2ps xmm1, xmm1
    ucomiss  xmm1, xmm0
    jp       .dec
    je       .done
.dec:
    dec      eax
.done:
    movd     xmm0, eax
    cvtdq2ps xmm0, xmm0
    ret
