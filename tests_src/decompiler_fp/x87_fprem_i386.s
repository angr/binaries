; x87 partial-remainder loops: fprem/fprem1 only reduce the exponent difference by up to 63 per step and set C2
; while the remainder is partial, so compilers repeat them until C2 clears.
; a = [esp+4], b = [esp+12]
bits 32

section .rodata
; (pi/2) * 2^63, the reduction modulus of MSVC's _CIsin
pio2_2_63: dt 1.4488038916154245685e+19

section .text

; MSVC _CIsin shape: fsin leaves the operand unchanged and sets C2 when |a| >= 2^63; reduce, then fsin again
global sin_reduce
sin_reduce:
    fld     qword [esp+4]
    fsin
    fnstsw  ax
    sahf
    jp      .reduce
    ret
.reduce:
    fld     tword [pio2_2_63]
    fxch    st1
.loop:
    fprem1
    fnstsw  ax
    sahf
    jp      .loop
    fstp    st1
    fsin
    ret

; gcc's fmod, with the C2 test spelled test ah, 4
global fmod_test_ah
fmod_test_ah:
    fld     qword [esp+12]
    fld     qword [esp+4]
.loop:
    fprem
    fnstsw  ax
    test    ah, 4
    jnz     .loop
    fstp    st1
    ret

; int: the low three quotient bits (C0, C3, C1) of the last fprem
global fmod_quotient_bits
fmod_quotient_bits:
    fld     qword [esp+12]
    fld     qword [esp+4]
.loop:
    fprem
    fnstsw  ax
    sahf
    jp      .loop
    fstp    st0
    fstp    st0
    shr     eax, 8
    and     eax, 0x43
    ret

; double: fmod(a, b), with the number of reduction steps stored to *(int *)[esp+20]; the loop must stay
global fmod_count
fmod_count:
    fld     qword [esp+12]
    fld     qword [esp+4]
    xor     ecx, ecx
.loop:
    inc     ecx
    fprem
    fnstsw  ax
    sahf
    jp      .loop
    fstp    st1
    mov     edx, [esp+20]
    mov     [edx], ecx
    ret
