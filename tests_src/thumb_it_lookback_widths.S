/*
 * Copyright (c) 2026 The angr Project developers
 * SPDX-License-Identifier: MIT
 */

	.syntax unified
	.cpu cortex-m3
	.thumb

	.text
	.p2align 1

	.global mixed_width_expired
	.type mixed_width_expired, %function
	.thumb_func
mixed_width_expired:
	push	{r4, r5, r6, r7, lr}
	cmp	r0, #0
	adds	r0, #1
	nop
	ittt	eq
	addeq	r0, r0, #1
	tsteq.w	r5, #0x18000
	addeq	r0, r0, #1
mixed_width_after:
	pop	{r4, r5, r6, r7, pc}
	.size mixed_width_expired, .-mixed_width_expired

	.p2align 1
	.global older_candidate_active
	.type older_candidate_active, %function
	.thumb_func
older_candidate_active:
	push	{r4, lr}
	cmp	r0, #0
	movs	r2, #0
	movs	r3, #0
	nop
	ittt	eq
	ldrexeq	r11, [r0, #32]
	addeq	r2, r2, #1
older_candidate_current:
	popeq	{r4, pc}
	mov	r0, r3
	pop	{r4, pc}
	.size older_candidate_active, .-older_candidate_active

	.p2align 1
	.global window_edge_expired
	.type window_edge_expired, %function
	.thumb_func
window_edge_expired:
	itttt	eq
	moveq.w	r1, #6
	moveq.w	r2, #6
	moveq.w	r3, #6
	moveq.w	r4, #6
window_edge_after:
	bx	lr
	.size window_edge_expired, .-window_edge_expired

	.section .note.GNU-stack,"",%progbits
