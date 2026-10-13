// aarch64-linux-gnu-as -o ../../tests/aarch64/decompiler/entry_jump_reentered.o entry_jump_reentered.s
// AArch64: the first block of drain_retry is only an unconditional branch, and the conditional
// branch at the end of the function jumps back to that first block.

    .text
    .globl drain_retry
    .type drain_retry, %function
drain_retry:
.Lretry:
    b       .Lcheck
.Lbody:
    sub     w1, w1, #1
    str     w1, [x0]
.Lcheck:
    ldr     w1, [x0]
    cbnz    w1, .Lbody
    ldr     w2, [x0, #4]
    cbnz    w2, .Lretry
    ret
    .size drain_retry, .-drain_retry

    .section .note.GNU-stack,"",%progbits
