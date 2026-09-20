# Paired R_MIPS_HI16 / R_MIPS_LO16 relocations against a section symbol, with a
# trailing %hi whose %lo never follows it.
#
# The last reference to bss_far loads only the high half of the address, so the
# R_MIPS_HI16 emitted for it is the last relocation in the table naming .bss.
# A consumer that insists on finding a later R_MIPS_LO16 against the same symbol
# has nothing to find. bss_near sits at .bss + 0x8000, so the low half of its
# address is negative once sign-extended and the high half has to compensate.

	.option	pic0
	.set	noreorder
	.set	noat

	.data
	.align	2
data_head:
	.word	0

	.bss
	.align	2
	.space	0x8000
bss_near:
	.space	4
	.space	0x4000
bss_far:
	.space	4

	.text
	.align	2
	.globl	hilo_pairs
hilo_pairs:
	lui	$8, %hi(data_head)
	lw	$9, %lo(data_head)($8)
	lui	$10, %hi(bss_near)
	lw	$11, %lo(bss_near)($10)
	lui	$12, %hi(bss_far)
	jr	$31
	 nop
