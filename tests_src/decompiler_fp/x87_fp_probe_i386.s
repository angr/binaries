; MSVC CRT _set_statfp-style helper: each flag bit runs an x87 op only to raise an FPU exception.
; The a0 & 4 branch (fldz; fld1; fdivrp; fstp st0) discards its result; the division must survive.
bits 32
section .rodata
huge: dt 1.0e+4000
tiny: dt 1.0e-4000

section .text

; void fp_raise(char flags)
global fp_raise
fp_raise:
    push    ebp
    mov     ebp, esp
    sub     esp, 16
    mov     ecx, [ebp+8]
    test    cl, 1
    je      .a
    fld     tword [huge]
    fistp   dword [ebp-4]
.a:
    test    cl, 8
    je      .b
    fld     tword [huge]
    fstp    qword [ebp-12]
.b:
    test    cl, 16
    je      .c
    fld     tword [tiny]
    fstp    qword [ebp-12]
.c:
    test    cl, 4
    je      .d
    fldz
    fld1
    fdivrp  st1
    fstp    st0
.d:
    test    cl, 32
    je      .e
    fldpi
    fstp    qword [ebp-12]
.e:
    leave
    ret

; double fp_div_used(void): 1.0 / 0.0 whose result is returned; must not be duplicated as a probe
global fp_div_used
fp_div_used:
    fldz
    fld1
    fdivrp  st1
    ret

; void fp_div_stored(double *p): 1.0 / 0.0 stored through p; must not be duplicated as a probe
global fp_div_stored
fp_div_stored:
    mov     eax, [esp+4]
    fldz
    fld1
    fdivrp  st1
    fstp    qword [eax]
    ret
