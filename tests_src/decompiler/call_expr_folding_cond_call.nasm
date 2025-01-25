bits 64
section .text

extern f2 

f1:
	cmp rdi, 3
	jne .not_3

	call f2

.not_3:
	ret
