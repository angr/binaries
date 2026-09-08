; X86 Windows x87 return values vs. incidental eax writes (__fastcall and __cdecl).
;
; fast_ret_double and cdecl_ret_double leave their result in st(0) while eax holds a scratch value, so the return
; type must come from the value left on the x87 stack. store_ret_int and trunc_int touch the x87 stack but leave it
; balanced (fld/fstp; a push that pop_arg consumes), so they return int. caller_fast_add reads st(0) only after a jmp,
; caller_consume_int pops the callee's value (it is not passed through) and caller_pass returns it unchanged.
;
; Assemble: nasm -f win32 -o x87_ret_proto_win32.obj x87_ret_proto_win32.s
;           i686-w64-mingw32-gcc -nostdlib -nostartfiles -Wl,-e,start -o x87_ret_proto_win32.exe x87_ret_proto_win32.obj
bits 32

section .text

global start
global fast_ret_double
global cdecl_ret_double
global store_ret_int
global pop_arg
global trunc_int
global caller_fast_add
global caller_consume_int
global caller_pass

; references every function so that CFG recovery finds them
start:
    call    store_ret_int
    call    trunc_int
    call    caller_fast_add
    call    caller_consume_int
    call    caller_pass
    xor     eax, eax
    ret

; double __fastcall fast_ret_double(struct { double d; int tag; } *p): eax = p->tag - 1 is scratch
fast_ret_double:
    mov     eax, [ecx + 8]
    sub     eax, 1
    jz      .one
    fld     qword [ecx]
    ret
.one:
    fld1
    ret

; double __cdecl cdecl_ret_double(double *p): st0 = *p, eax = 1
cdecl_ret_double:
    mov     eax, [esp + 4]
    fld     qword [eax]
    mov     eax, 1
    ret

; int __fastcall store_ret_int(double *p): p[1] = p[0] through the x87 stack; returns 3
store_ret_int:
    fld     qword [ecx]
    fstp    qword [ecx + 8]
    mov     eax, 3
    ret

; int pop_arg(void): pops st0 into eax (like MSVC _ftol2)
pop_arg:
    sub     esp, 4
    fistp   dword [esp]
    pop     eax
    ret

; int __fastcall trunc_int(double *p): return pop_arg(*p)
trunc_int:
    fld     qword [ecx]
    call    pop_arg
    ret

; void __fastcall caller_fast_add(struct *p, double *q): *q += fast_ret_double(p); st(0) is read only after a jmp
caller_fast_add:
    push    esi
    mov     esi, edx
    call    fast_ret_double
    jmp     .add
.add:
    fadd    qword [esi]
    fstp    qword [esi]
    pop     esi
    ret

; int __cdecl caller_consume_int(double *p): *p = cdecl_ret_double(p); return 0
caller_consume_int:
    mov     ecx, [esp + 4]
    push    ecx
    call    cdecl_ret_double
    add     esp, 4
    mov     ecx, [esp + 4]
    fstp    qword [ecx]
    xor     eax, eax
    ret

; double __cdecl caller_pass(double *p): return cdecl_ret_double(p)
caller_pass:
    push    dword [esp + 4]
    call    cdecl_ret_double
    add     esp, 4
    ret
