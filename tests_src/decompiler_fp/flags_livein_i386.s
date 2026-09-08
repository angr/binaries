; Branches on eflags live on function entry (no instruction in the function sets them).
; build: nasm -f elf32 -o flags_livein_i386.o flags_livein_i386.s
bits 32
section .text

; jb on the entry CF
global livein_jb
livein_jb:
    mov     eax, 1
    jb      .t
    mov     eax, 2
.t:
    ret

; jnl on the entry flags: SF == OF
global livein_jnl
livein_jnl:
    mov     eax, 1
    jnl     .t
    mov     eax, 2
.t:
    ret
