; fld m80 followed by fstp m64 / fistp m32: the narrowing store is the point (it raises overflow/underflow)
bits 32
section .rodata
huge: dt 1.0e+4000
tiny: dt 1.0e-4000

section .text

; void narrow_store(char flags): one double slot written from long doubles and from fldpi
global narrow_store
narrow_store:
    push    ebp
    mov     ebp, esp
    sub     esp, 16
    mov     ecx, [ebp+8]
    test    cl, 1
    je      .a
    fld     tword [huge]
    fstp    qword [ebp-12]
.a:
    test    cl, 2
    je      .b
    fld     tword [tiny]
    fstp    qword [ebp-12]
.b:
    test    cl, 4
    je      .c
    fldpi
    fstp    qword [ebp-12]
.c:
    leave
    ret

; int narrow_fistp(void): long double -> int32
global narrow_fistp
narrow_fistp:
    push    ebp
    mov     ebp, esp
    sub     esp, 8
    fld     tword [huge]
    fistp   dword [ebp-4]
    mov     eax, [ebp-4]
    leave
    ret
