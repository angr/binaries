; A small Windows 3.x program for cle's NE loader tests.
;
; Two movable code segments and a fixed automatic data segment, so that the
; linker emits far calls between segments (through the entry table), a far
; pointer held in data, a selector loaded into a register, and calls into
; KERNEL and USER imported both by ordinal and by name. ShowCaption is
; exported, and it calls MessageBox twice.

        .286

        public  ShowCaption

        extrn   INITTASK:far
        extrn   WAITEVENT:far
        extrn   INITAPP:far
        extrn   MESSAGEBOX:far

DGROUP  group   _DATA

_DATA   segment word public 'DATA'
caption db      'cle', 0
message db      'NE fixture', 0
handler dd      ShowCaption
_DATA   ends

MAIN_TEXT segment word public 'CODE'
        assume  cs:MAIN_TEXT, ds:DGROUP
start:
        call    INITTASK
        or      ax, ax
        jz      failed
        xor     ax, ax
        push    ax
        call    WAITEVENT
        push    di
        call    INITAPP
        mov     ax, seg message
        mov     ds, ax
        call    far ptr ShowCaption
        call    dword ptr handler
        mov     ax, 4c00h
        int     21h
failed:
        mov     ax, 4c01h
        int     21h
MAIN_TEXT ends

MSG_TEXT segment word public 'CODE'
        assume  cs:MSG_TEXT, ds:DGROUP
ShowCaption proc far
        push    bp
        mov     bp, sp
        xor     ax, ax
        push    ax
        push    ds
        mov     ax, offset DGROUP:message
        push    ax
        push    ds
        mov     ax, offset DGROUP:caption
        push    ax
        xor     ax, ax
        push    ax
        call    MESSAGEBOX
        xor     ax, ax
        push    ax
        push    ds
        mov     ax, offset DGROUP:caption
        push    ax
        push    ds
        mov     ax, offset DGROUP:message
        push    ax
        xor     ax, ax
        push    ax
        call    MESSAGEBOX
        pop     bp
        retf
ShowCaption endp
MSG_TEXT ends

        end     start
