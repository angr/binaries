; i386 functions that take their arguments on the x87 stack and pop them, like MSVC's _CI* and _ftol helpers.
;
; ci_sin replaces st(0) with its sine (an in-place return: net x87 stack effect 0). drop2_one pops two x87 arguments
; and returns 1.0 (like the MSVC CRT error-return helpers); tail_drop2 tail-jumps to it. ftol_disp chooses between an
; SSE conversion and a tail jump to pop_int (an _ftol2-style dispatcher). The callers surface the x87 values as call
; arguments and keep the st(0) results.
;
; Assemble: nasm -f elf32 -o x87_stack_args_i386.o x87_stack_args_i386.s

    section .text
    global ci_sin
    global drop2_one
    global tail_drop2
    global pop_int
    global ftol_disp
    global caller_sin
    global caller_two
    global caller_ftol

; double ci_sin(double x /* st0 */)
ci_sin:
    fsin
    ret

; double drop2_one(double x /* st0 */, double y /* st1 */)
drop2_one:
    fstp st0
    fstp st0
    fld1
    ret

; double tail_drop2(double x /* st0 */, double y /* st1 */)
tail_drop2:
    jmp drop2_one

; int pop_int(double x /* st0 */)
pop_int:
    sub esp, 4
    fistp dword [esp]
    pop eax
    ret

; int ftol_disp(double x /* st0 */)
ftol_disp:
    cmp dword [use_sse], 0
    je pop_int
    sub esp, 8
    fstp qword [esp]
    cvttsd2si eax, qword [esp]
    add esp, 8
    ret

; void caller_sin(double *p): p[1] = ci_sin(p[0]) * 2.0
caller_sin:
    mov eax, [esp+4]
    fld qword [eax]
    call ci_sin
    fadd st0, st0
    mov eax, [esp+4]
    fstp qword [eax+8]
    ret

; void caller_two(double *p): p[2] = tail_drop2(p[1], p[0])
caller_two:
    mov eax, [esp+4]
    fld qword [eax]
    fld qword [eax+8]
    call tail_drop2
    mov eax, [esp+4]
    fstp qword [eax+16]
    ret

; int caller_ftol(double *p): ftol_disp(*p) + 1
caller_ftol:
    mov eax, [esp+4]
    fld qword [eax]
    call ftol_disp
    inc eax
    ret

    section .bss
use_sse: resd 1
