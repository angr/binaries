; A real-mode DOS program used as the stub of a PE test fixture.
;
; wasm emits an OMF object, and wlink emits the MZ executable. The initialized
; data makes the linked stub larger than 4 KiB without reserving space in BSS
; or modifying the executable after linking.

        .8086

_DATA   segment word public 'DATA'
padding db      4096 dup(1)
_DATA   ends

_TEXT   segment para public 'CODE'
        assume  cs:_TEXT

start:
        mov     ax, 4C00h
        int     21h

_TEXT   ends

        end     start
