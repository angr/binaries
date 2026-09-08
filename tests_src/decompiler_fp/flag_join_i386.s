; A jcc whose flags come from compares in two predecessors (jump threading), plus stmxcsr/fnstcw reads.
; build: nasm -f elf32 -o flag_join_i386.o flag_join_i386.s
bits 32
section .text

extern sse2_path
extern x87_path

; the MSVC SSE2-dispatch check: (mxcsr & 0x7f80) == 0x1f80 && (fpucw & 0x7f) == 0x7f
global sse2_dispatch
sse2_dispatch:
    sub     esp, 8
    stmxcsr [esp+4]
    mov     eax, [esp+4]
    and     eax, 0x7f80
    cmp     eax, 0x1f80
    jne     .join
    fnstcw  [esp]
    mov     ax, [esp]
    and     ax, 0x7f
    cmp     ax, 0x7f
.join:
    lea     esp, [esp+8]
    jne     .x87
    call    sse2_path
    ret
.x87:
    call    x87_path
    ret

; integer-only: a == 5 && b == 7 ? 1 : 2, the second jne threaded through a shared block
global int_flag_join
int_flag_join:
    mov     eax, [esp+4]
    cmp     eax, 5
    jne     .join
    mov     ecx, [esp+8]
    cmp     ecx, 7
.join:
    lea     edx, [eax+1]
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret
