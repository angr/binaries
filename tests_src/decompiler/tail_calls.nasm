; nasm -felf64 -o tail_calls.o tail_calls.nasm

section .text

;
; Unconditional tailcall
;

global test_tailcall
global test_tailcall_callee

test_tailcall:
    inc edi
    jmp test_tailcall_callee

test_tailcall_callee:
    mov eax, edi
    add eax, 20
    ret

call_test_tailcall:
    ; Force function creation
    call test_tailcall
    call test_tailcall_callee

;
; Unconditional non-returning tailcall
;

global test_noreturn_tailcall
global test_noreturn_tailcall_callee

test_noreturn_tailcall:
    inc edi
    jmp test_noreturn_tailcall_callee

test_noreturn_tailcall_callee:
.loop:
    jmp .loop

call_test_noreturn_tailcall:
    ; Force function creation
    call test_noreturn_tailcall
    call test_noreturn_tailcall_callee

;
; Unconditional fallthru
;

global test_fallthru
global test_fallthru_callee

test_fallthru:
    inc edi
    ; fallthru...

test_fallthru_callee:
    mov eax, edi
    add eax, 20
    ret

call_test_fallthru:
    ; Force function creation
    call test_fallthru
    call test_fallthru_callee

;
; Unconditional non-returning fallthru
;

global test_noreturn_fallthru
global test_noreturn_fallthru_callee

test_noreturn_fallthru:
    inc edi
    ; fallthru...

test_noreturn_fallthru_callee:
.loop:
    jmp .loop

call_test_noreturn_fallthru:
    ; Force function creation
    call test_noreturn_fallthru
    call test_noreturn_fallthru_callee

;
; Conditional tailcall via unconditional jump
;

global test_cond_tailcall_jmp
global test_cond_tailcall_jmp_callee

test_cond_tailcall_jmp:
    test edi, edi
    jnz .do_tailcall
    mov eax, edi
    dec eax
    ret
.do_tailcall:
    jmp test_cond_tailcall_jmp_callee

test_cond_tailcall_jmp_callee:
    mov eax, edi
    add eax, 10
    ret

call_test_cond_tailcall_jmp:
    ; Force function creation
    call test_cond_tailcall_jmp
    call test_cond_tailcall_jmp_callee
    ret

;
; Conditional non-returning tailcall via unconditional jump
;

global test_cond_noreturn_tailcall_jmp
global test_cond_noreturn_tailcall_jmp_callee

test_cond_noreturn_tailcall_jmp:
    test edi, edi
    jnz .do_tailcall
    mov eax, edi
    dec eax
    ret
.do_tailcall:
    jmp test_cond_noreturn_tailcall_jmp_callee

test_cond_noreturn_tailcall_jmp_callee:
.loop:
    jmp .loop

call_test_cond_noreturn_tailcall_jmp:
    ; Force function creation
    call test_cond_noreturn_tailcall_jmp
    call test_cond_noreturn_tailcall_jmp_callee
    ret

;
; Conditional tailcall via conditional jump
;

global test_cond_tailcall_cjmp
global test_cond_tailcall_cjmp_callee

test_cond_tailcall_cjmp:
    test edi, edi
    jnz test_cond_tailcall_cjmp_callee
    mov eax, edi
    dec eax
    ret

test_cond_tailcall_cjmp_callee:
    mov eax, edi
    add eax, 10
    ret

call_test_cond_tailcall_cjmp:
    ; Force function creation
    call test_cond_tailcall_cjmp
    call test_cond_tailcall_cjmp_callee
    ret

;
; Conditional non-returning tailcall via conditional jump
;

global test_cond_noreturn_tailcall_cjmp
global test_cond_noreturn_tailcall_cjmp_callee

test_cond_noreturn_tailcall_cjmp:
    test edi, edi
    jnz test_cond_noreturn_tailcall_cjmp_callee
    mov eax, edi
    dec eax
    ret

test_cond_noreturn_tailcall_cjmp_callee:
.loop:
    jmp .loop

call_test_cond_noreturn_tailcall_cjmp:
    ; Force function creation
    call test_cond_noreturn_tailcall_cjmp
    call test_cond_noreturn_tailcall_cjmp_callee
    ret

;
; Conditional tailcall via conditional jump with fallthru to another function
;

global test_cond_tailcall_cjmp_fallthru
global test_cond_tailcall_cjmp_fallthru_callee_fallthru
global test_cond_tailcall_cjmp_fallthru_callee_tailcall

test_cond_tailcall_cjmp_fallthru:
    test edi, edi
    jnz test_cond_tailcall_cjmp_fallthru_callee_tailcall
    ; fallthru...

test_cond_tailcall_cjmp_fallthru_callee_fallthru:
    mov eax, edi
    add eax, 10
    ret

test_cond_tailcall_cjmp_fallthru_callee_tailcall:
    mov eax, edi
    add eax, 20
    ret

call_test_cond_tailcall_cjmp_fallthru:
    ; Force function creation
    call test_cond_tailcall_cjmp_fallthru
    call test_cond_tailcall_cjmp_fallthru_callee_fallthru
    call test_cond_tailcall_cjmp_fallthru_callee_tailcall
    ret


;
; Conditional non-returning tailcall via conditional jump with fallthru to another function
;

global test_cond_noreturn_tailcall_cjmp_fallthru
global test_cond_noreturn_tailcall_cjmp_fallthru_callee_fallthru
global test_cond_noreturn_tailcall_cjmp_fallthru_callee_tailcall

test_cond_noreturn_tailcall_cjmp_fallthru:
    test edi, edi
    jnz test_cond_noreturn_tailcall_cjmp_fallthru_callee_tailcall
    ; fallthru...

test_cond_noreturn_tailcall_cjmp_fallthru_callee_fallthru:
    mov eax, edi
    add eax, 10
    ret

test_cond_noreturn_tailcall_cjmp_fallthru_callee_tailcall:
.loop:
    jmp .loop

call_test_cond_noreturn_tailcall_cjmp_fallthru:
    ; Force function creation
    call test_cond_noreturn_tailcall_cjmp_fallthru
    call test_cond_noreturn_tailcall_cjmp_fallthru_callee_fallthru
    call test_cond_noreturn_tailcall_cjmp_fallthru_callee_tailcall
    ret
