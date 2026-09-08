; Microsoft x64: a strtold-style parser that does SSE math internally but returns status flags in eax,
; and a helper that reads xmm0 right after a call returning a double.
;   double fetch(double *p);
;   double scale(double *p);                       -- xmm0 read after `call fetch` is fetch's result, not an argument
;   int parse(char *s, char **end, char *sep, int *exp, unsigned long long *mant);  -- 5th arg at [rsp+0x28]
;   void *caller(void *out, char *s, char **end);  -- switch (parse(...) & 7), sign bit in flag 8
; Build: nasm -f win64 -o flags_ret_win64.obj flags_ret_win64.s &&
;        x86_64-w64-mingw32-gcc -nostdlib -nostartfiles -Wl,-e,start -o flags_ret_win64.exe flags_ret_win64.obj
bits 64
default rel

section .text

global start
global fetch
global scale
global parse
global caller

start:
    xor     eax, eax
    ret

fetch:
    movsd   xmm0, [rcx]
    ret

scale:
    sub     rsp, 0x28
    call    fetch
    mulsd   xmm0, [two]
    add     rsp, 0x28
    ret

parse:
    push    rbx
    push    rsi
    push    rdi
    sub     rsp, 0x40
    movups  [rsp + 0x30], xmm6
    mov     rbx, rcx                    ; s
    mov     [rsp + 0x68], rdx           ; end -> its home slot
    mov     rsi, r9                     ; exp
    mov     rdi, [rsp + 0x80]           ; mant (5th argument)
    movzx   eax, byte [rbx]
    cmp     al, '-'
    jne     .digits
    mov     dword [rsi], 0
    mov     qword [rdi], 0
    mov     eax, 0xb                    ; flags: kind 3, negative
    jmp     .done
.digits:
    movzx   eax, byte [rbx]
    sub     eax, '0'
    pxor    xmm0, xmm0
    cvtsi2sd xmm0, eax
    mulsd   xmm0, [ten]
    movapd  xmm6, xmm0
    mov     rcx, r8
    call    scale
    addsd   xmm6, xmm0
    cvttsd2si rax, xmm6
    mov     [rdi], rax
    mov     dword [rsi], 1
    mov     rax, [rsp + 0x68]
    mov     [rax], rbx
    mov     eax, 6
.done:
    movups  xmm6, [rsp + 0x30]
    add     rsp, 0x40
    pop     rdi
    pop     rsi
    pop     rbx
    ret

caller:
    push    rbx
    sub     rsp, 0x50
    mov     rbx, rcx                    ; out
    lea     rax, [rsp + 0x38]
    mov     [rsp + 0x20], rax           ; 5th argument: &mant
    lea     r9, [rsp + 0x34]            ; &exp
    mov     rcx, rdx                    ; s
    mov     rdx, r8                     ; end
    lea     r8, [sep]
    call    parse
    mov     edx, eax
    and     eax, 7
    cmp     eax, 3
    jne     .store
    mov     qword [rsp + 0x38], 0x7fff8000
.store:
    mov     rax, [rsp + 0x38]
    mov     [rbx], rax
    mov     eax, [rsp + 0x34]
    mov     [rbx + 8], eax
    and     edx, 8
    je      .ret
    or      word [rbx + 8], 0x8000
.ret:
    mov     rax, rbx
    add     rsp, 0x50
    pop     rbx
    ret

section .rdata
two:    dq 2.0
ten:    dq 10.0
sep:    db "@", 0
