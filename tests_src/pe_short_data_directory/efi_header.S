/*
 * PE/COFF header for a minimal UEFI application, written the way real EFI
 * stub images write it (cf. Linux arch/arm64/kernel/efi-header.S and
 * arch/x86/boot/header.S): the assembler computes NumberOfRvaAndSizes from
 * the number of data directory entries that are actually present, so a stub
 * that only declares the first six directories reports 6, not 16.
 */

	.set	SECTION_ALIGNMENT, 0x1000
	.set	FILE_ALIGNMENT, 0x1000

	.section .head, "a"
	.globl	ImageBase
ImageBase:
	/* MS-DOS stub: only the magic and e_lfanew matter to a PE loader. */
	.ascii	"MZ"
	.skip	58
	.long	pe_header - ImageBase		/* e_lfanew, at offset 0x3c */

pe_header:
	.ascii	"PE\0\0"

coff_header:
	.short	0x8664				/* Machine: IMAGE_FILE_MACHINE_AMD64 */
	.short	section_count			/* NumberOfSections */
	.long	0				/* TimeDateStamp */
	.long	0				/* PointerToSymbolTable */
	.long	0				/* NumberOfSymbols */
	.short	section_table - optional_header	/* SizeOfOptionalHeader */
	.short	0x0206				/* EXECUTABLE_IMAGE | LINE_NUMS_STRIPPED | DEBUG_STRIPPED */

optional_header:
	.short	0x20b				/* PE32+ */
	.byte	0x02				/* MajorLinkerVersion */
	.byte	0x14				/* MinorLinkerVersion */
	.long	_text_size			/* SizeOfCode */
	.long	_data_size			/* SizeOfInitializedData */
	.long	0				/* SizeOfUninitializedData */
	.long	efi_main			/* AddressOfEntryPoint (ImageBase is 0) */
	.long	_text				/* BaseOfCode */

	.quad	0				/* ImageBase */
	.long	SECTION_ALIGNMENT		/* SectionAlignment */
	.long	FILE_ALIGNMENT			/* FileAlignment */
	.short	0				/* MajorOperatingSystemVersion */
	.short	0				/* MinorOperatingSystemVersion */
	.short	0				/* MajorImageVersion */
	.short	0				/* MinorImageVersion */
	.short	0				/* MajorSubsystemVersion */
	.short	0				/* MinorSubsystemVersion */
	.long	0				/* Win32VersionValue */
	.long	_image_size			/* SizeOfImage */
	.long	_headers_size			/* SizeOfHeaders */
	.long	0				/* CheckSum */
	.short	10				/* Subsystem: EFI_APPLICATION */
	.short	0				/* DllCharacteristics */
	.quad	0				/* SizeOfStackReserve */
	.quad	0				/* SizeOfStackCommit */
	.quad	0				/* SizeOfHeapReserve */
	.quad	0				/* SizeOfHeapCommit */
	.long	0				/* LoaderFlags */
	.long	(section_table - .) / 8		/* NumberOfRvaAndSizes */

	/* Only six data directory entries follow, so the expression above
	 * evaluates to (4 + 6 * 8) / 8 == 6. */
	.quad	0				/* ExportTable */
	.quad	0				/* ImportTable */
	.quad	0				/* ResourceTable */
	.quad	0				/* ExceptionTable */
	.quad	0				/* CertificateTable */
	.quad	0				/* BaseRelocationTable */

section_table:
	.ascii	".text\0\0\0"
	.long	_text_size			/* VirtualSize */
	.long	_text				/* VirtualAddress */
	.long	_text_size			/* SizeOfRawData */
	.long	_text				/* PointerToRawData */
	.long	0				/* PointerToRelocations */
	.long	0				/* PointerToLinenumbers */
	.short	0				/* NumberOfRelocations */
	.short	0				/* NumberOfLinenumbers */
	.long	0x60000020			/* CODE | EXECUTE | READ */

	.ascii	".data\0\0\0"
	.long	_data_size			/* VirtualSize */
	.long	_data				/* VirtualAddress */
	.long	_data_size			/* SizeOfRawData */
	.long	_data				/* PointerToRawData */
	.long	0				/* PointerToRelocations */
	.long	0				/* PointerToLinenumbers */
	.short	0				/* NumberOfRelocations */
	.short	0				/* NumberOfLinenumbers */
	.long	0xc0000040			/* INITIALIZED_DATA | READ | WRITE */

	.set	section_count, (. - section_table) / 40

	/* Mark the stack non-executable; the section itself is discarded. */
	.section .note.GNU-stack, "", @progbits
