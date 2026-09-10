; dos_com.asm -- a small MS-DOS .COM program, built as an angr/cle test fixture.
;
; Built with NASM: nasm -f bin. A .COM has no header at all: the file is the
; load module, mapped at offset 0100h in a single segment, entered at its first
; byte, with CS = DS = ES = SS. So a loader has no MZ header to parse and must
; treat the whole file as one flat 16-bit real-mode blob based at 0x100.
;
; It prints two messages with INT 21h / AH=09h and exits with INT 21h / AH=4Ch.
; There are two near calls into two named functions so that a CFG has something
; to find beyond the entry block.

        cpu     8086
        bits    16
        org     0100h

start:
        mov     dx, msg_hello
        call    print_msg               ; near call

        mov     si, msg_hello
        call    count_chars             ; near call
        mov     [char_count], cx

        mov     dx, msg_bye
        call    print_msg               ; near call

        mov     ax, 4C00h               ; exit(0)
        int     21h

; Write the '$'-terminated string at DS:DX.
print_msg:
        mov     ah, 09h
        int     21h
        ret

; Count the bytes of the '$'-terminated string at DS:SI, result in CX.
count_chars:
        xor     cx, cx
.loop:
        mov     al, [si]
        cmp     al, '$'
        je      .done
        inc     si
        inc     cx
        jmp     .loop
.done:
        ret

msg_hello   db  'COM fixture: hello from real mode.', 0Dh, 0Ah, '$'
msg_bye     db  'COM fixture: goodbye.', 0Dh, 0Ah, '$'

            align 2
char_count  dw  0
