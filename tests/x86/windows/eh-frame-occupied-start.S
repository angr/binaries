/*
 * Copyright (c) 2026 The angr Project Developers
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * A minimal MinGW PE fixture with a GNU .eh_frame FDE for a function reached
 * by a tail jump. Without the FDE, CFG recovery owns the target as part of the
 * wrapper instead of preserving it as a separate function.
 */

    .text
    .globl _start
_start:
    call tail_wrapper
    ret

tail_wrapper:
    xorl %ecx, %ecx
    jmp hidden_function

hidden_function:
    .cfi_startproc
    movl $42, %eax
    ret
    .cfi_endproc
