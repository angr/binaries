; x87 status words stored to memory as values (not consumed by a flag test)
; a = [esp+4], b = [esp+12]
; Assemble: nasm -f elf32 -o x87_fnstsw_store_i386.o x87_fnstsw_store_i386.s
bits 32

section .bss
global g_sw
g_sw:   resw 1

section .text

; ftst with a0 on the stack (ftop = 7): g_sw = 0x3800 | C3/C2/C0 of a0 vs 0.0
global ftst_store
ftst_store:
    fld     qword [esp+4]
    ftst
    fnstsw  word [g_sw]
    fstp    st0
    ret

; fcom st0 = a, src = b: CmpF(a, b)
global fcom_store
fcom_store:
    fld     qword [esp+4]
    fcom    qword [esp+12]
    fnstsw  word [g_sw]
    fstp    st0
    ret

; fucompp st0 = b, st1 = a: CmpF(b, a); ftop is back to 0 when the status word is read
global fucompp_ax_store
fucompp_ax_store:
    fld     qword [esp+4]
    fld     qword [esp+12]
    fucompp
    fnstsw  ax
    mov     [g_sw], ax
    ret
