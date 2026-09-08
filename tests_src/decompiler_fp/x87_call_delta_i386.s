; i386 x87 stack tracking across calls (IRegisterResolver), plus fistp/fisttp fptag ITEs, a segment-override
; long double load and fxam.
;
; ret_double also writes eax, so calling-convention analysis may type it as returning int; the resolver must derive
; the st0 push from the callee's own code. pop_arg consumes its x87 argument (like MSVC _ftol2). ext_fn is extern, so
; the push is inferred from the caller's first x87 access after the call. caller_unbalanced merges paths that leave
; different x87 depths; every access must still resolve to a concrete st(i).
;
; Assemble: nasm -f elf32 -o x87_call_delta_i386.o x87_call_delta_i386.s

    section .text
    global ret_double
    global pop_arg
    global void_fn
    global caller_merge
    global caller_pop
    global caller_extern
    global caller_unbalanced
    global fistp_word
    global fld_f80_seg
    global fisttp_then_shl
    global fxam_fn
    extern ext_fn

; double ret_double(double *p): st0 = *p, eax = 1
ret_double:
    mov eax, [esp+4]
    fld qword [eax]
    mov eax, 1
    ret

; int pop_arg(void): pops st0 into eax
pop_arg:
    sub esp, 4
    fistp dword [esp]
    pop eax
    ret

void_fn:
    ret

; void caller_merge(double *p, int flag)
caller_merge:
    push ebp
    mov ebp, esp
    sub esp, 8
    mov ecx, [ebp+8]
    cmp dword [ebp+12], 3
    je .merge
    push ecx
    call ret_double
    add esp, 4
    fstp qword [ebp-8]
    call void_fn
    fld qword [ebp-8]
    fstp qword [ecx]
.merge:
    push ecx
    call ret_double
    add esp, 4
    fsubr qword [ecx]
    fstp qword [ecx]
    leave
    ret

; int caller_pop(double *p): pop_arg(*p); *p = 1.0
caller_pop:
    mov ecx, [esp+4]
    fld qword [ecx]
    call pop_arg
    fld1
    fstp qword [ecx]
    ret

; void caller_extern(double *p): *p = ext_fn()
caller_extern:
    call ext_fn
    mov ecx, [esp+4]
    fstp qword [ecx]
    ret

; void caller_unbalanced(double *p, int flag)
caller_unbalanced:
    mov ecx, [esp+4]
    cmp dword [esp+8], 0
    je .one
    fld qword [ecx]
    fld1
    jmp .merge
.one:
    fldz
.merge:
    faddp st1, st0
    fstp qword [ecx]
    ret

; void fistp_word(double *p, short *q): *q = (short)*p
fistp_word:
    mov eax, [esp+4]
    mov ecx, [esp+8]
    fld qword [eax]
    fistp word [ecx]
    ret

; long double fld_f80_seg(long double *p): segment-override load (address from a ccall)
fld_f80_seg:
    mov eax, [esp+4]
    fld tword [fs:eax]
    ret

; void fisttp_then_shl(double *p, long long *q, char *r, int n): *q = (long long)*p; r[0x100] <<= n
; the shift-by-cl ITE is rewritten into a diamond; the fisttp fptag ITE before it in the same block must survive
fisttp_then_shl:
    mov eax, [esp+4]
    mov edx, [esp+8]
    mov ecx, [esp+16]
    fld qword [eax]
    fisttp qword [edx]
    mov eax, [esp+12]
    shl byte [eax+0x100], cl
    ret

; unsigned int fxam_fn(double *p): classify ret_double(p) with fxam; the x87 tag of the returned value is unknown
fxam_fn:
    push dword [esp+4]
    call ret_double
    add esp, 4
    fxam
    fnstsw ax
    fstp st0
    and eax, 0x4500
    ret
