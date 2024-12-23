bits 64
section .text


global main
main:
    push rbp
    mov rbp, 0x12345678
    
    mov qword [rbp], 0
    
    call f1

    mov qword [rbp], 1
    
    test rax, rax
    mov rax, 0
    jz .end
    mov rax, 1
.end:
    pop rbp
    ret

global f1
f1:
    mov rax, [0x12345678]
    xor rax, 1
    ret
