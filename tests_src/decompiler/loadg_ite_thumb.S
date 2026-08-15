/*
 * Copyright (c) 2026 The angr Project developers
 * SPDX-License-Identifier: MIT
 */

	.syntax unified
	.cpu cortex-m3
	.thumb

	.text
	.p2align 1
	.global loadg_ite_mask
	.type loadg_ite_mask, %function
	.thumb_func
loadg_ite_mask:
	movs	r3, #0
	lsls	r1, r1, #31
	ite	eq
	ldreq	r3, [r0]
	ldrne	r3, [r0, #4]
	ands	r3, r2
	mov	r0, r3
	bx	lr
	.size loadg_ite_mask, .-loadg_ite_mask

	.section .note.GNU-stack,"",%progbits
