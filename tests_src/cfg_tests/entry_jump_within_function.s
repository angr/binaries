.text
.globl rotated_loop
.type rotated_loop, @function
rotated_loop:
    jmp .Lguard
.Lbody:
    addl $1, (%rdi)
.Lguard:
    cmpl $4, (%rdi)
    jne .Lbody
    ret
.size rotated_loop, .-rotated_loop

.section .note.GNU-stack,"",@progbits
