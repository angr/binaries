/* Fixture for the zero-size normalization anchor defect.

   Two basic blocks overlap and end at the same address, and that address
   holds a UDF (permanently undefined) instruction that VEX decodes to a
   zero-byte IRSB.  CFGFast still creates a real (non-SimProcedure) CFGNode
   for it, so the normalization pass groups all three by end address, picks
   the zero-extent node as the one to split the others at, and splits
   nothing.  */

    .arch armv7-a
    .syntax unified
    .arm
    .text

    .global _start
    .type _start, %function
_start:
    mov  r0, #2
    bl   entangle
    mov  r7, #1
    svc  #0
    .size _start, .-_start

    .global entangle
    .type entangle, %function
entangle:
    cmp  r0, #1
    beq  .Ltop            @ discovers the wide block first
    cmp  r0, #0
    beq  .Lmid            @ then discovers a block starting inside it
    bx   lr

.Ltop:
    add  r0, r0, #1       @ wide block: two instructions
.Lmid:
    b    .Lund            @ narrow block: one instruction; both end at .Lund

.Lund:
    .word 0xe7f000f0      @ UDF #0 -- decodes to nothing
    .word 0xe7f000f0

    bx   lr
    .size entangle, .-entangle

    .section .note.GNU-stack,"",%progbits
