; X86 Windows: int __fastcall copy64(long long *p, int unused, long long v, int order), an MSVC inlined
; InterlockedExchange64-style store. The two dwords of v are spilled to a stack slot and moved with fild/fistp qword,
; an exact int64 copy; two of the paths share the fistp block (the x87 register is live across the edge) and eax keeps
; the low dword as the return value. The C must be a plain int64 copy, not an Insert reassembly plus
; (long long)(double).
;
; Assemble: nasm -f win32 -o x87_int64_copy_win32.obj x87_int64_copy_win32.s
;           i686-w64-mingw32-gcc -nostdlib -nostartfiles -Wl,-e,start -o x87_int64_copy_win32.exe x87_int64_copy_win32.obj
bits 32

section .text

global start
global copy64

start:
    push    0
    push    0
    push    0
    xor     edx, edx
    lea     ecx, [esp - 8]
    call    copy64
    ret

copy64:
    push    ebp
    mov     ebp, esp
    push    ecx
    push    ecx
    mov     eax, [ebp + 16]
    mov     edx, ecx
    sub     eax, 0
    je      .a
    sub     eax, 1
    je      .c
    sub     eax, 1
    je      .c
    sub     eax, 1
    je      .b
    sub     eax, 1
.c:
    nop
    mov     eax, [ebp + 8]
    mov     ecx, [ebp + 12]
    mov     [ebp - 8], eax
    mov     [ebp - 4], ecx
    fild    qword [ebp - 8]
    fistp   qword [edx]
    nop
    lock inc dword [ebp - 4]
    nop
    jmp     .out
.b:
    nop
    mov     eax, [ebp + 8]
    mov     ecx, [ebp + 12]
    mov     [ebp - 8], eax
    mov     [ebp - 4], ecx
    fild    qword [ebp - 8]
    jmp     .st
.a:
    mov     eax, [ebp + 8]
    mov     ecx, [ebp + 12]
    mov     [ebp - 8], eax
    mov     [ebp - 4], ecx
    fild    qword [ebp - 8]
.st:
    fistp   qword [edx]
.out:
    leave
    ret     12
