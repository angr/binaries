; SP-relative loads/stores whose address is computed before an sp update (lea esp,[esp+N] / add esp,N)
; but consumed after it in AIL.
; build: nasm -f elf32 -o sp_lea_i386.o sp_lea_i386.s
; a = [esp+4], b = [esp+8] (ints); x = [esp+4], y = [esp+12] (doubles)
bits 32
section .text

; (a & 0x4100) != 0 ? 2 : 1; the flags of `test` outlive `lea esp`, so the load lands in the jump condition
global int_test_lea
int_test_lea:
    mov     eax, [esp+4]
    push    eax
    test    byte [esp+1], 0x41
    lea     esp, [esp+4]
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

; same, with the store and the test in different blocks
global int_test_lea_2blk
int_test_lea_2blk:
    sub     esp, 4
    mov     eax, [esp+8]
    mov     [esp], eax
    cmp     dword [esp+12], 0
    je      .l
    nop
.l:
    test    byte [esp+1], 0x41
    lea     esp, [esp+4]
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

; control: add esp after the branch
global int_test_add_2blk
int_test_add_2blk:
    sub     esp, 4
    mov     eax, [esp+8]
    mov     [esp], eax
    cmp     dword [esp+12], 0
    je      .l
    nop
.l:
    test    byte [esp+1], 0x41
    jne     .no
    mov     eax, 1
    add     esp, 4
    ret
.no:
    mov     eax, 2
    add     esp, 4
    ret

; a = 7; return a  (store through an address taken before the sp update)
global int_store_copy_lea
int_store_copy_lea:
    sub     esp, 4
    lea     edx, [esp+8]
    lea     esp, [esp+4]
    mov     dword [edx], 7
    mov     eax, [esp+4]
    ret

global int_store_copy_add
int_store_copy_add:
    sub     esp, 4
    lea     edx, [esp+8]
    add     esp, 4
    mov     dword [edx], 7
    mov     eax, [esp+4]
    ret

; x <= y (or unordered) -> 2 else 1; fnstsw word reloaded across lea esp
global fcomp_local_lea
fcomp_local_lea:
    sub     esp, 4
    fld     qword [esp+8]
    fcomp   qword [esp+16]
    fnstsw  word [esp]
    test    byte [esp+1], 0x41
    lea     esp, [esp+4]
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

; same, with fnstsw and the test in different blocks
global fcomp_local_lea_2blk
fcomp_local_lea_2blk:
    sub     esp, 4
    fld     qword [esp+8]
    fcomp   qword [esp+16]
    fnstsw  word [esp]
    cmp     dword [esp+24], 0
    je      .l
    nop
.l:
    test    byte [esp+1], 0x41
    lea     esp, [esp+4]
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret
