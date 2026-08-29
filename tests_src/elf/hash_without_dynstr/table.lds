/* Keeps .hash and .dynsym while discarding .dynstr, as u-boot's own linker
   scripts do. The linker then writes sh_link = 0 into both of them. */
ENTRY(_start)
SECTIONS
{
	. = 0x1000;
	.text : { *(.text*) }
	.rodata : { *(.rodata*) }
	.data : { *(.data*) }
	.bss : { *(.bss*) *(COMMON) }
	.hash : { *(.hash) }
	.dynsym : { *(.dynsym) }
	.rela.dyn : { *(.rela*) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.note*) }
	/DISCARD/ : { *(.comment) }
	/DISCARD/ : { *(.eh_frame*) }
}
