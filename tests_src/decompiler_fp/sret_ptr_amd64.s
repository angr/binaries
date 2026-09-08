; SysV x64 hidden result pointer: ld_sret writes a 10-byte x87 value through rdi and returns rdi in rax on every
; path (an sret-style prototype: uint80_t *ld_sret(uint80_t *, ...)). bump_void only leaves a leftover in eax and
; stays void. use_both calls both and ignores their results.
; Assemble: nasm -f elf64 -o sret_ptr_amd64.o sret_ptr_amd64.s
bits 64
section .text
global ld_sret, bump_void, use_both

; uint80_t *ld_sret(uint80_t *res, const uint80_t *src, int neg): *res = neg ? -*src : *src; return res
ld_sret:
    push    rbx
    mov     rbx, rdi
    fld     tword [rsi]
    test    edx, edx
    jz      .pos
    fchs
    fstp    tword [rbx]
    mov     rax, rbx
    pop     rbx
    ret
.pos:
    fstp    tword [rbx]
    mov     rax, rbx
    pop     rbx
    ret

; void bump_void(int *p): *p += 1; eax holds the stored value at ret
bump_void:
    mov     eax, dword [rdi]
    add     eax, 1
    mov     dword [rdi], eax
    ret

; void use_both(int *p, const uint80_t *src)
use_both:
    push    rbx
    sub     rsp, 16
    mov     rbx, rdi
    mov     rdi, rsp
    mov     edx, 1
    call    ld_sret
    mov     rdi, rbx
    call    bump_void
    add     rsp, 16
    pop     rbx
    ret
