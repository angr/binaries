; _ftol2 shape: fsubp before the branch, identical fstp dword [esp] + reload in both successors
bits 32
section .text

; int sibling_store(double x)
global sibling_store
sibling_store:
    push    ebp
    mov     ebp, esp
    sub     esp, 0x20
    and     esp, 0xfffffff0
    fld     qword [ebp+8]
    fld     st0
    fst     dword [esp+0x18]
    fistp   qword [esp+0x10]
    fild    qword [esp+0x10]
    mov     edx, [esp+0x18]
    mov     eax, [esp+0x10]
    fsubp   st1, st0
    test    edx, edx
    jns     .pos
    fstp    dword [esp]
    mov     ecx, [esp]
    xor     ecx, 0x80000000
    add     ecx, 0x7fffffff
    adc     eax, 0
    jmp     .done
.pos:
    fstp    dword [esp]
    mov     ecx, [esp]
    add     ecx, 0x7fffffff
    sbb     eax, 0
.done:
    leave
    ret
