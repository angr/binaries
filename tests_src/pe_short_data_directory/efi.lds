OUTPUT_FORMAT("elf64-x86-64")
ENTRY(efi_main)
SECTIONS
{
	. = 0;
	ImageBase = .;
	.head : { *(.head) . = ALIGN(0x1000); }

	_text = .;
	.text : { *(.text) *(.text.*) . = ALIGN(0x1000); }
	_etext = .;

	_data = .;
	.data : { *(.rodata) *(.rodata.*) *(.data) *(.data.*) *(.got) *(.got.plt) . = ALIGN(0x1000); }
	_edata = .;

	_end = .;

	_text_size = _etext - _text;
	_data_size = _edata - _data;
	_headers_size = _text - ImageBase;
	_image_size = _end - ImageBase;

	/DISCARD/ : { *(.note*) *(.comment) *(.eh_frame*) *(.dynamic) *(.dynsym) *(.dynstr) *(.hash) *(.gnu.hash) *(.rela*) *(.symtab) *(.strtab) }
}
