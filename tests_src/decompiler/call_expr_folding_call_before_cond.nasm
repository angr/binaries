bits 64

extern f2

f1:
	call f2

	cmp rdi, 3
	jne .not_three
.three:
	ret

.not_three:
	xor rax, rax
	ret	

	
