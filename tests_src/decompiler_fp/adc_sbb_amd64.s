; adc/sbb carry chains at 64 bits: the carry-in of every adc/sbb after the first is amd64g_calculate_rflags_c over an
; ADC/SBB thunk.
; build: nasm -f elf64 -o adc_sbb_amd64.o adc_sbb_amd64.s
bits 64
section .text

; void add128(uint64_t w[2], uint64_t lo, uint64_t hi): w += hi:lo
global add128
add128:
    add     [rdi], rsi
    adc     [rdi+8], rdx
    ret

; void sub128(uint64_t w[2], uint64_t lo, uint64_t hi): w -= hi:lo
global sub128
sub128:
    sub     [rdi], rsi
    sbb     [rdi+8], rdx
    ret

; void add_chain(uint64_t w[3], uint64_t v): add w[0], v; adc w[1], 0; adc w[2], 0
global add_chain
add_chain:
    add     [rdi], rsi
    adc     qword [rdi+8], 0
    adc     qword [rdi+16], 0
    ret

; void sub_chain(uint64_t w[3], uint64_t v): sub w[0], v; sbb w[1], 0; sbb w[2], 0
global sub_chain
sub_chain:
    sub     [rdi], rsi
    sbb     qword [rdi+8], 0
    sbb     qword [rdi+16], 0
    ret

; uint64_t adc_jc(uint64_t a, uint64_t b, uint64_t c): a + b + c, ~0 if the final adc carries
global adc_jc
adc_jc:
    mov     rax, rdi
    add     rax, rsi
    adc     rax, rdx
    jc      .ovf
    ret
.ovf:
    mov     rax, -1
    ret
