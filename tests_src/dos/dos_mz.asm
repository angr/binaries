; dos_mz.asm -- a small real-mode MS-DOS .EXE, built as an angr/cle test fixture.
;
; Built with Open Watcom 1.9: wasm (assembler) -> OMF object -> wlink (linker) -> MZ .EXE.
; Nothing about the MZ header or the relocation table is hand-written; wlink emits both.
;
; The program is a legitimate DOS program: it prints three messages with
; INT 21h / AH=09h and exits with INT 21h / AH=4Ch, return code 0.
;
; It is shaped so that a loader has to get the interesting parts of MZ right:
;
;   * five segment-value fixups land in the MZ relocation table (e_crlc == 5):
;       1. "mov ax, DGROUP"           - group segment value, in code
;       2. "call far ptr far_hello"   - segment word of a far call target, in code
;       3. "mov ax, seg FAR_TEXT"     - segment value of a second code segment, in code
;       4. "far_ptr   dd far_bye"     - segment word of a far pointer stored in DATA
;       5. "hello_ptr dd far_hello"   - segment word of a second far pointer in DATA
;     Three are in the code segment and two are in _DATA, so a loader that only
;     relocates the code segment gets it wrong. The table also carries two
;     different frame segments (0000 and 0028), not one.
;
;     wlink must be told "option nofarcalls": its far-call optimisation otherwise
;     rewrites "call far ptr" into "push cs / call near" and deletes fixup 2.
;     wasm is likewise given -zld so it emits no far-call-optimisation records.
;
;   * the entry point is not at offset 0 of the load module: _DATA is linked
;     first, so initial CS > 0, and print_msg sits ahead of "start" inside _TEXT,
;     so initial IP > 0 too.
;
;   * there is a real stack segment, so initial SS:SP is non-zero.
;
;   * four identifiable functions: two near (print_msg, count_chars) and two far
;     (far_hello, far_bye), reached by a direct near call, a direct far call, and
;     an indirect far call through the relocated pointer in _DATA.

        .8086

DGROUP  group   _DATA

; ---------------------------------------------------------------- data ------

_DATA   segment word public 'DATA'

msg_hello   db  'MZ fixture: hello from real mode.', 0Dh, 0Ah, '$'
msg_far     db  'far_hello ran in FAR_TEXT.', 0Dh, 0Ah, '$'
msg_bye     db  'far_bye ran through a relocated far pointer.', 0Dh, 0Ah, '$'

                align 2
far_ptr     dd  far_bye         ; RELOCATION: segment word of this far pointer
hello_ptr   dd  far_hello       ; RELOCATION: segment word of a second far pointer
char_count  dw  0

_DATA   ends

; --------------------------------------------------------------- stack ------

STACK   segment para stack 'STACK'
        db      256 dup(?)
STACK   ends

; ------------------------------------------------- second code segment ------

FAR_TEXT segment para public 'FAR_CODE'
        assume  cs:FAR_TEXT, ds:DGROUP

; Print msg_far. Called with a direct far call from _TEXT.
far_hello proc far
        push    ax
        push    dx
        mov     dx, offset DGROUP:msg_far
        mov     ah, 09h
        int     21h
        pop     dx
        pop     ax
        ret
far_hello endp

; Print msg_bye. Reached only through the relocated far pointer in _DATA.
far_bye proc far
        push    ax
        push    dx
        mov     dx, offset DGROUP:msg_bye
        mov     ah, 09h
        int     21h
        pop     dx
        pop     ax
        ret
far_bye endp

FAR_TEXT ends

; ---------------------------------------------------------------- code ------

_TEXT   segment para public 'CODE'
        assume  cs:_TEXT, ds:DGROUP, ss:STACK

; Write the '$'-terminated string at DS:DX. Near call target, defined ahead of
; the entry point so that the initial IP is non-zero.
print_msg proc near
        mov     ah, 09h
        int     21h
        ret
print_msg endp

; Count the bytes of the '$'-terminated string at DS:SI, result in CX.
; A second near function, so a CFG has more than one near edge to find.
count_chars proc near
        xor     cx, cx
count_loop:
        mov     al, [si]
        cmp     al, '$'
        je      count_done
        inc     si
        inc     cx
        jmp     count_loop
count_done:
        ret
count_chars endp

start:
        mov     ax, DGROUP              ; RELOCATION: group segment value
        mov     ds, ax
        assume  ds:DGROUP

        mov     dx, offset DGROUP:msg_hello
        call    print_msg               ; near call

        mov     si, offset DGROUP:msg_hello
        call    count_chars             ; near call
        mov     char_count, cx

        call    far ptr far_hello       ; RELOCATION: segment of a far call

        mov     ax, seg FAR_TEXT        ; RELOCATION: segment of FAR_TEXT
        mov     es, ax

        call    dword ptr far_ptr       ; indirect far call through the relocated pointer

        mov     ax, 4C00h               ; exit(0)
        int     21h

_TEXT   ends

        end     start
