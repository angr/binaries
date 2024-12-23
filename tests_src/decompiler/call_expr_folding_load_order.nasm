bits 64
section .text


global main
main:
    push rbp

    mov qword [0x12345678], 1

    call f1
    mov rbx, rax

    xor rax, rax

    mov rcx, qword [0x12345678]
    test rcx, rcx
    jz .end

    test rbx, rbx
    jz .end

    mov rax, 1
.end:
    pop rbp
    ret

global f1
f1:
    xor rax, rax
    mov [0x12345678], rax
    inc rax
    ret
