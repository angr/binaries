; fptan: st(0) = tan(x), then push 1.0. VEX lifts the push as conditional on the argument being in range
; (maybe_fp_push), so ftop after fptan is data-dependent.
bits 32
section .text

; double tan_ret(double x): return tan(x)
global tan_ret
tan_ret:
    fld     qword [esp+4]
    fptan
    fstp    st0
    ret

; void tan_store(double *p): *p = tan(*p)
global tan_store
tan_store:
    mov     eax, [esp+4]
    fld     qword [eax]
    fptan
    fstp    st0
    fstp    qword [eax]
    ret

; double tan_plus(double x, double y): return tan(x) + y
global tan_plus
tan_plus:
    fld     qword [esp+4]
    fptan
    fstp    st0
    fadd    qword [esp+12]
    ret
