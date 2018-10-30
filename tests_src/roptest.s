.global _start
.data
.ascii "this is just a test"

.text
_start:
.intel_syntax noprefix
	pop rax; ret
	pop rdx; ret
	mov qword ptr [rax + 8], rdx; ret
	int3; int3
	pop rdi; ret
	pop rdx; ret
	pop rsi; ret
	syscall
