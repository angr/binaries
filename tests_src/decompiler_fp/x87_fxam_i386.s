; x87 status-word idioms: fxam classification tests and fnstsw to memory
; build: nasm -f elf32 -o x87_fxam_i386.o x87_fxam_i386.s
; x = [esp+4], y = [esp+12]
bits 32
section .text

%macro FXAM_AH 0
    fld     qword [esp+4]
    fxam
    fnstsw  ax
    fstp    st0
%endmacro

global fxam_isnan
fxam_isnan:
    FXAM_AH
    and     ah, 0x45
    cmp     ah, 1
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global fxam_isinf
fxam_isinf:
    FXAM_AH
    and     ah, 0x45
    cmp     ah, 5
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global fxam_iszero
fxam_iszero:
    FXAM_AH
    and     ah, 0x45
    cmp     ah, 0x40
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global fxam_isnormal
fxam_isnormal:
    FXAM_AH
    and     ah, 0x45
    cmp     ah, 4
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global fxam_signbit
fxam_signbit:
    FXAM_AH
    test    ah, 2
    je      .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

; C0 -> CF: NaN or infinity
global fxam_notfinite_sahf
fxam_notfinite_sahf:
    FXAM_AH
    sahf
    jnc     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

; MSVC CRT style: the status word goes to the caller's frame and its high byte is reloaded
global fxam_mem_notfinite
fxam_mem_notfinite:
    fld     qword [esp+4]
    fxam
    fnstsw  word [ebp-0xa0]
    fstp    st0
    test    byte [ebp-0x9f], 1
    je      .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global ftst_mem_le
ftst_mem_le:
    fld     qword [esp+4]
    ftst
    fnstsw  word [ebp-0xa0]
    fstp    st0
    test    byte [ebp-0x9f], 0x41
    je      .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

; fnstsw to a local stack slot
global fcomp_local_lt
fcomp_local_lt:
    sub     esp, 4
    fld     qword [esp+8]
    fcomp   qword [esp+16]
    fnstsw  word [esp]
    test    byte [esp+1], 1
    je      .no
    mov     eax, 1
    add     esp, 4
    ret
.no:
    mov     eax, 2
    add     esp, 4
    ret
