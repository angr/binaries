bits 64
section .text


global main
main:
    push rbp

    call f1
    mov rbp, rax

    call f2

    cmp rax, rbp
    mov rax, 0
    jne .end
    call f3
.end:
    pop rbp
    ret

global f1
f1:
    mov rax, 1
    ret

global f2
f2:
    mov rax, 1
    ret

global f3
f3:
    ret
