; A small OS/2 1.x program for cle's NE loader tests.
;
; It writes a line to standard output and exits, calling DOSCALLS by ordinal.

        .286

        extrn   DOSWRITE:far
        extrn   DOSEXIT:far

DGROUP  group   _DATA, STACK

_DATA   segment word public 'DATA'
message db      'NE fixture', 13, 10
written dw      0
_DATA   ends

STACK   segment para stack 'STACK'
STACK   ends

_TEXT   segment word public 'CODE'
        assume  cs:_TEXT, ds:DGROUP
start:
        push    1
        push    ds
        push    offset DGROUP:message
        push    written - message
        push    ds
        push    offset DGROUP:written
        call    DOSWRITE
        push    1
        push    0
        call    DOSEXIT
_TEXT   ends

        end     start
