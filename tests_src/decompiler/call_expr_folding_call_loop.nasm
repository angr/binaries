bits 64
section .text

global main
main:
	push rbp
	xor rbp, rbp
.cond:
	cmp rbp, 3
	jge .end
.body:
	call f1
	add rbp, 1
	jmp .cond
.end:
	pop rbp
	ret

global f1
f1:
	ret
