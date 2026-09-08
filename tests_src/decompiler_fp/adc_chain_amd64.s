; A chain of 8-bit adc's on a partial register: every adc's left operand is the previous sum, which is not a
; virtual variable of its own (al lives inside rax) and gets propagated into the next carry thunk. Rewriting such a
; carry into a formula that repeats its operands grows the expression exponentially along the chain.
; build: nasm -f elf64 -o adc_chain_amd64.o adc_chain_amd64.s
bits 64
section .text

; uint8_t adc_chain8(uint8_t a, uint8_t b, uint8_t c, uint8_t d)
global adc_chain8
adc_chain8:
    mov     al, dil
    add     al, sil
    adc     al, dl
    adc     al, cl
    adc     al, sil
    adc     al, dl
    adc     al, cl
    adc     al, sil
    adc     al, dl
    adc     al, cl
    adc     al, sil
    adc     al, dl
    adc     al, cl
    ret
