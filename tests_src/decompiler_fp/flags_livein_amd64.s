; Branches and flag reads on rflags live on function entry (no instruction in the function sets them).
; build: nasm -f elf64 -o flags_livein_amd64.o flags_livein_amd64.s
bits 64
section .text

; je on the entry ZF
global livein_je
livein_je:
    mov     eax, 1
    je      .t
    mov     eax, 2
.t:
    ret

; jle on the entry flags: ZF | (SF != OF)
global livein_jle
livein_jle:
    mov     eax, 1
    jle     .t
    mov     eax, 2
.t:
    ret

; adc reads the entry CF
global livein_adc
livein_adc:
    mov     eax, edi
    adc     eax, 0
    ret

; setz stores the entry ZF as a value
global livein_setz
livein_setz:
    setz    al
    movzx   eax, al
    ret

; jle at a loop head: entry flags on the first iteration, set by dec afterwards; not rewritten
global partial_loop
partial_loop:
    mov     eax, edi
.l:
    jle     .done
    dec     eax
    jmp     .l
.done:
    ret
