; dos_mz_tiny.asm -- a real-mode MS-DOS .EXE in the "tiny model" layout,
; where the initial CS *and* the initial SS in the MZ header are NEGATIVE
; paragraph displacements, and the stack top sits exactly at the start of the
; load module.
;
; Built with flat assembler (fasm) 1.73.35:
;     fasm dos_mz_tiny.asm hello_tiny.exe
; The MZ header is produced entirely by fasm's "format MZ" writer; nothing here
; hand-writes a header field.
;
; What makes it interesting to a loader:
;
;   e_cs = 0FFF0h, e_ip = 0100h,  e_ss = 0FFF0h, e_sp = 0100h.
;
; DOS loads the load module at PSP+10h and then computes the initial CS and SS
; as load_segment + the header field, in 16-bit arithmetic. A linker is free to
; write a field that wraps, and 0FFF0h means "sixteen paragraphs below the load
; module", which is where the PSP sits. So CS = SS = DS = ES = the PSP segment,
; and CS:0100h is offset 0 of the load module -- the .COM memory model wearing
; an MZ header. The program is assembled at "org 100h" so every label is
; already PSP-relative.
;
; Two things a loader has to get right, and this one file exercises both:
;
;   1. The wrap. Read as unsigned, 0FFF0h:0100h is 0FFF0h * 10h + 100h =
;      100000h, which is one byte past the top of the 20-bit real-mode address
;      space and nowhere near a 160-byte file.
;
;   2. The boundary. That unsigned reading lands on 100000h *exactly*, and the
;      wrapped reading lands on 0 exactly. A loader that resolves the wrap by
;      trying both readings and taking whichever is in range must make the
;      upper bound of "in range" exclusive of the real-mode space, or both
;      readings pass and it has nothing to choose between them.
;
; The program itself is an ordinary DOS program: it prints two messages with
; INT 21h / AH=09h and exits with INT 21h / AH=4Ch, return code 0. There are
; two near-called functions so a CFG has something to find beyond the entry
; block. There are no relocations (e_crlc = 0): nothing here loads a segment
; register with a linked segment value.
;
; On the stack. SS:SP = PSP:0100h means the stack grows down from offset 100h
; into the PSP itself. That is safe here with room to spare, and it was measured
; rather than assumed: a probe program in this same shape, doing this same
; work, was run under DOSBox with the PSP painted, and its stack reached
; PSP:00F8 (two bytes for a near-call return address, six for an INT frame),
; and even a generous allowance for the register saves a real MS-DOS INT 21h
; dispatcher makes on the caller's stack, plus a hardware interrupt frame, does
; not get below PSP:0080. Everything from 0080h up is the command tail and the
; default DTA, which this program never reads. The first byte that would matter
; is FCB #1 at 005Ch, 164 bytes below the initial SP, and every field DOS
; itself needs -- the saved INT 22h/23h/24h vectors, the parent PSP, the job
; file table, the environment pointer, DOS's own SS:SP save slot -- is below
; that again. Do not add recursion or a stack buffer to this program without
; redoing that measurement.

format MZ

; SS:SP -- the same negative displacement as CS, with the stack top at offset
; 100h of the PSP segment, i.e. offset 0 of the load module. fasm writes
; 0FFF0h:0100h into e_ss:e_sp.
stack code_seg-10h:0100h

segment code_seg use16

; Everything is addressed relative to the PSP, which is where CS points, so the
; first byte of the load module sits at offset 100h -- as in a .COM.
org 100h

; CS = load_segment - 10h, IP = 100h. fasm writes 0FFF0h into e_cs and 0100h
; into e_ip.
entry code_seg-10h:start

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
count_loop:
        mov     al, [si]
        cmp     al, '$'
        je      count_done
        inc     si
        inc     cx
        jmp     count_loop
count_done:
        ret

msg_hello   db  'Tiny-model MZ fixture: hello from real mode.', 0Dh, 0Ah, '$'
msg_bye     db  'Tiny-model MZ fixture: goodbye.', 0Dh, 0Ah, '$'

            align 2
char_count  dw  0

; Claim the rest of the 64 KB segment. fasm writes none of it into the file --
; an uninitialised tail is what sets e_minalloc -- so the header asks DOS for
; the whole segment and the file stays 160 bytes. e_minalloc comes out as
; 0FE8h paragraphs.
            rb  0FFFEh - $
