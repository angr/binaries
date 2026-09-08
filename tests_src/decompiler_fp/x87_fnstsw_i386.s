; MSVC-style x87 comparison idioms: fcomp; fnstsw ax; test ah, imm / sahf; jcc
; a = [esp+4], b = [esp+12]
bits 32
section .text

global lt_test_jp
lt_test_jp:
    fld     qword [esp+4]
    fcomp   qword [esp+12]
    fnstsw  ax
    test    ah, 5
    jp      .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global gt_test_jne
gt_test_jne:
    fld     qword [esp+4]
    fcomp   qword [esp+12]
    fnstsw  ax
    test    ah, 0x41
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global eq_test_jnp
eq_test_jnp:
    fld     qword [esp+4]
    fcomp   qword [esp+12]
    fnstsw  ax
    test    ah, 0x44
    jnp     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global lt_test_jne
lt_test_jne:
    fld     qword [esp+4]
    fcomp   qword [esp+12]
    fnstsw  ax
    test    ah, 1
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global ge_sahf_jb
ge_sahf_jb:
    fld     qword [esp+4]
    fcomp   qword [esp+12]
    fnstsw  ax
    sahf
    jb      .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global le_sahf_ja
le_sahf_ja:
    fld     qword [esp+4]
    fcomp   qword [esp+12]
    fnstsw  ax
    sahf
    ja      .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global eq_sahf_jne
eq_sahf_jne:
    fld     qword [esp+4]
    fcomp   qword [esp+12]
    fnstsw  ax
    sahf
    jne     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global isnan_sahf_jnp
isnan_sahf_jnp:
    fld     qword [esp+4]
    fucomp  st0
    fnstsw  ax
    sahf
    jnp     .no
    mov     eax, 1
    ret
.no:
    mov     eax, 2
    ret

global lt_sahf_setb
lt_sahf_setb:
    fld     qword [esp+4]
    fcomp   qword [esp+12]
    fnstsw  ax
    sahf
    setb    al
    movzx   eax, al
    ret
