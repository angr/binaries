; i386 cdecl: double lane_cmp_phi(double x, double y, int k)
; cmpeqsd/pextrw test lane 0 of a mask that also flows, alongside a movlpd lane-0 Insert, into a phi whose only
; read is lane 0 (the CRT log() shape). The C must test x == 0.0 and read x for the mulsd, not 128-bit vectors.
;
;   if (x == 0.0) r = y; else if (k) return x * x; else r = mask(x == 0.0);
;   return r;
;
; Assemble: nasm -f elf32 -o sse_phi_insert_i386.o sse_phi_insert_i386.s

    section .text
    global lane_cmp_phi

lane_cmp_phi:
    sub esp, 8
    movlpd xmm0, [esp+12]
    xorpd xmm1, xmm1
    cmpeqsd xmm1, xmm0
    pextrw eax, xmm1, 0
    cmp eax, 0
    ja .zero
    cmp dword [esp+28], 0
    je .merge
    mulsd xmm0, xmm0
    movlpd [esp], xmm0
    fld qword [esp]
    add esp, 8
    ret
.zero:
    movlpd xmm1, [esp+20]
.merge:
    movlpd [esp], xmm1
    fld qword [esp]
    add esp, 8
    ret
