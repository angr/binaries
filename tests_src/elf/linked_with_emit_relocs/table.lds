/* An ordinary linker script for a fixed-address image: nothing unusual in the
   section table, so every ELF reader parses it. What matters is that the image
   is linked at 0x1000 rather than at 0, and that --emit-relocs leaves the
   link-time .rel sections behind the way u-boot's build does. */
ENTRY(_start)
SECTIONS
{
	. = 0x1000;
	.text : { *(.text*) }
	.rodata : { *(.rodata*) }
	.data : { *(.data*) }
	.bss : { *(.bss*) *(COMMON) }
	.rel.dyn : { *(.rel.dyn) }
	/DISCARD/ : { *(.hash) }
	/DISCARD/ : { *(.dynsym) }
	/DISCARD/ : { *(.dynstr*) }
	/DISCARD/ : { *(.dynamic*) }
	/DISCARD/ : { *(.interp*) }
	/DISCARD/ : { *(.gnu*) }
	/DISCARD/ : { *(.note*) }
	/DISCARD/ : { *(.comment) }
	/DISCARD/ : { *(.eh_frame*) }
}
