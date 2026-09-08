; X86 Windows _dclass-style classifier: reads its double argument through the x87 stack, returns a small integer
; code in eax/ax (one path writes only `mov ax, 0xffff`) and pops the x87 stack before returning. The return value is
; an integer, not a float. caller tests ax after each call.
;
; Assemble: nasm -f win32 -o x87_dclass_win32.obj x87_dclass_win32.s
;           i686-w64-mingw32-gcc -nostdlib -nostartfiles -Wl,-e,start -o x87_dclass_win32.exe x87_dclass_win32.obj
bits 32

section .text

global start
global dclass
global caller

start:
    push    0
    push    0
    call    caller
    add     esp, 8
    ret

; short __cdecl dclass(double x)
dclass:
    push    ebp
    mov     ebp, esp
    and     esp, 0xfffffff8
    sub     esp, 0x10
    fld     qword [ebp + 8]
    fst     qword [esp + 8]
    mov     ecx, [esp + 0xc]
    mov     edx, ecx
    shr     edx, 0x14
    and     edx, 0x7ff
    cmp     edx, 0x7ff
    je      .nan_inf
    mov     ax, 0xffff
    test    edx, edx
    jne     .out
    fstp    qword [esp]
    mov     eax, 0x7fffffff
    and     eax, [esp + 4]
    xor     ecx, ecx
    or      eax, [esp]
    sete    cl
    lea     eax, [ecx * 2 - 2]
    jmp     .zero
.nan_inf:
    fstp    st0
    and     ecx, 0xfffff
    xor     eax, eax
    or      ecx, [esp + 8]
    setne   al
    inc     eax
.zero:
    fldz
.out:
    fstp    st0
    mov     esp, ebp
    pop     ebp
    ret

; int __cdecl caller(double x): 1 if dclass(x) == 2, 3 if == 1, else 0
caller:
    push    ebp
    mov     ebp, esp
    fld     qword [ebp + 8]
    sub     esp, 8
    fstp    qword [esp]
    call    dclass
    add     esp, 8
    cmp     ax, 2
    je      .two
    fld     qword [ebp + 8]
    sub     esp, 8
    fstp    qword [esp]
    call    dclass
    add     esp, 8
    cmp     ax, 1
    je      .one
    xor     eax, eax
    pop     ebp
    ret
.two:
    mov     eax, 1
    pop     ebp
    ret
.one:
    mov     eax, 3
    pop     ebp
    ret
