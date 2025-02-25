; nasm -felf64 -o regs_preserved_across_syscalls.o regs_preserved_across_syscalls.nasm
; ld -o regs_preserved_across_syscalls regs_preserved_across_syscalls.o

bits 64


section .rodata

msg0: db "hello"
.len equ $ - msg0

msg1: db " world", 0xA
.len equ $ - msg1


section .text

print_hello_world:
	push rbx

	mov ebx, 1
	lea rsi, [rel msg0]
	mov edx, msg0.len
	mov eax, ebx
	mov edi, ebx
	syscall

	lea rsi, [rel msg1]
	mov eax, ebx
	mov edx, msg1.len
	syscall

	pop rbx
	xor rax, rax
	ret

global _start
_start:
	call print_hello_world
	mov eax, 60
	mov rdi, 123
	syscall
