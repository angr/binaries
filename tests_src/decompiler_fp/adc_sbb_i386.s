; adc/sbb carry chains: the carry-in of every adc/sbb after the first is x86g_calculate_eflags_c over an ADC/SBB thunk.
; build: nasm -f elf32 -o adc_sbb_i386.o adc_sbb_i386.s
bits 32
section .text

; uint64_t add64(uint64_t a, uint64_t b): add/adc, result in edx:eax
global add64
add64:
    mov     eax, [esp+4]
    mov     edx, [esp+8]
    add     eax, [esp+12]
    adc     edx, [esp+16]
    ret

; uint64_t sub64(uint64_t a, uint64_t b): sub/sbb, result in edx:eax
global sub64
sub64:
    mov     eax, [esp+4]
    mov     edx, [esp+8]
    sub     eax, [esp+12]
    sbb     edx, [esp+16]
    ret

; void add_chain(uint32_t w[3], uint32_t v): add w[0], v; adc w[1], 0; adc w[2], 0 (the MSVC _ftol2 shape)
global add_chain
add_chain:
    mov     ecx, [esp+4]
    mov     edx, [esp+8]
    add     [ecx], edx
    adc     dword [ecx+4], 0
    adc     dword [ecx+8], 0
    ret

; void sub_chain(uint32_t w[3], uint32_t v): sub w[0], v; sbb w[1], 0; sbb w[2], 0
global sub_chain
sub_chain:
    mov     ecx, [esp+4]
    mov     edx, [esp+8]
    sub     [ecx], edx
    sbb     dword [ecx+4], 0
    sbb     dword [ecx+8], 0
    ret

; uint32_t adc_jc(uint32_t a, uint32_t b, uint32_t c): a + b + c, 0xffffffff if the final adc carries
global adc_jc
adc_jc:
    mov     eax, [esp+4]
    add     eax, [esp+8]
    adc     eax, [esp+12]
    jc      .ovf
    ret
.ovf:
    mov     eax, 0xffffffff
    ret

; uint32_t sbb_jz(uint32_t a, uint32_t b, uint32_t c): 1 if a - b - c == 0 (jz after sbb), else 0
global sbb_jz
sbb_jz:
    mov     eax, [esp+4]
    sub     eax, [esp+8]
    sbb     eax, [esp+12]
    jz      .zero
    xor     eax, eax
    ret
.zero:
    mov     eax, 1
    ret
