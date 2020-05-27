stuff:
	LDR R3, =things
	LDR R6, [R3]
	LDR R1, =0xBEEF
	ADD R1, R2
	MOV R0, R6
lol:    B lol
things:
	.word 0x40001234

