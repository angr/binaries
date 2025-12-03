; nasm -felf64 -o ../../tests/x86_64/decompiler/intrinsics.o intrinsics.nasm

bits 64
section .text

global test_io_outb
test_io_outb:
    mov al, dil
    out 0xe9, al
    ret

global test_io_inb
test_io_inb:
    in al, 0xe9
    movzx eax, al
    ret

global test_io_outw
test_io_outw:
    mov dx, di
    mov ax, si
    out dx, ax
    ret

global test_io_inw
test_io_inw:
    mov dx, di
    in ax, dx
    ret

global test_io_outl
test_io_outl:
    mov eax, edi
    mov dx, 0xcf8
    out dx, eax
    mov eax, esi
    mov dx, 0xcfc
    out dx, eax
    ret

global test_io_inl
test_io_inl:
    mov eax, edi
    mov dx, 0xcf8
    out dx, eax
    mov dx, 0xcfc
    in eax, dx
    ret

global test_in_cond
test_in_cond:
    in al, 0xe9
    test al, al
    jz .is_zero
    mov eax, 123
    ret
.is_zero:
    mov eax, 456
    ret
