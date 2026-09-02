#!/usr/bin/env python3
"""
Builds the minimal COFF objects that cle's COFF loader tests use.

No object emitted by a real toolchain in this repository has a long section name, a relocation
field that wraps, a relocation field positioned outside the section that owns it or outside the
file, or an ARM64 or ARMNT machine type, so these are assembled by hand.
"""

from __future__ import annotations

import os
import struct

IMAGE_FILE_MACHINE_I386 = 0x14C
IMAGE_FILE_MACHINE_ARMNT = 0x1C4
IMAGE_FILE_MACHINE_R4000 = 0x166
IMAGE_FILE_MACHINE_ARM64 = 0xAA64

IMAGE_REL_I386_DIR32 = 0x0006
IMAGE_REL_I386_REL32 = 0x0014

IMAGE_REL_ARM_ADDR32 = 0x0001
IMAGE_REL_ARM_MOV32T = 0x0011
IMAGE_REL_ARM_BRANCH24T = 0x0014

IMAGE_REL_ARM64_ADDR32NB = 0x0002
IMAGE_REL_ARM64_BRANCH26 = 0x0003
IMAGE_REL_ARM64_PAGEBASE_REL21 = 0x0004
IMAGE_REL_ARM64_PAGEOFFSET_12A = 0x0006
IMAGE_REL_ARM64_PAGEOFFSET_12L = 0x0007
IMAGE_REL_ARM64_ADDR64 = 0x000E

IMAGE_SYM_TYPE_FUNCTION = 0x20
IMAGE_SYM_CLASS_EXTERNAL = 2
IMAGE_SCN_TEXT = 0x60000020  # CNT_CODE | MEM_EXECUTE | MEM_READ

COFF_HEADER = struct.Struct("<HHLLLHH")
COFF_SECTION_HEADER = struct.Struct("<8sLLLLLLHHL")
COFF_SYMBOL = struct.Struct("<8sLhHBB")
COFF_RELOCATION = struct.Struct("<LLH")

TESTS_DIR = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", "..", "tests")


class CoffObjectWriter:
    """
    Assembles a minimal COFF object in memory.
    """

    def __init__(self, machine: int = IMAGE_FILE_MACHINE_I386):
        self._machine = machine
        self._sections: list[dict] = []
        self._symbols: list[tuple[str, int, int, int]] = []
        self._string_table = bytearray()

    def add_section(self, name: str, data: bytes) -> int:
        """
        Append a section and return its one-based section number.
        """
        self._sections.append({"name": name, "data": data, "relocations": []})
        return len(self._sections)

    def add_symbol(self, name: str, value: int, section_number: int, symbol_type: int = 0) -> int:
        """
        Append an external symbol defined at `value` bytes into `section_number`, and return its
        symbol table index. Section number 0 makes it undefined.
        """
        self._symbols.append((name, value, section_number, symbol_type))
        return len(self._symbols) - 1

    def add_relocation(self, section_number: int, offset: int, symbol_index: int, reloc_type: int) -> None:
        self._sections[section_number - 1]["relocations"].append((offset, symbol_index, reloc_type))

    def build(self) -> bytes:
        raw_data_end = self._section_offset(len(self._sections) + 1)

        section_headers = bytearray()
        raw_data = bytearray()
        relocation_tables = bytearray()
        for section_number, section in enumerate(self._sections, start=1):
            relocations = section["relocations"]
            section_headers += COFF_SECTION_HEADER.pack(
                self._section_name(section["name"]),
                0,  # VirtualSize
                0,  # VirtualAddress
                len(section["data"]),  # SizeOfRawData
                self._section_offset(section_number),  # PointerToRawData
                raw_data_end + len(relocation_tables) if relocations else 0,  # PointerToRelocations
                0,  # PointerToLinenumbers
                len(relocations),  # NumberOfRelocations
                0,  # NumberOfLinenumbers
                IMAGE_SCN_TEXT,  # Characteristics
            )
            raw_data += section["data"]
            for offset, symbol_index, reloc_type in relocations:
                relocation_tables += COFF_RELOCATION.pack(offset, symbol_index, reloc_type)

        symbol_table = bytearray()
        for name, value, section_number, symbol_type in self._symbols:
            symbol_table += COFF_SYMBOL.pack(
                self._symbol_name(name),
                value,
                section_number,
                symbol_type,
                IMAGE_SYM_CLASS_EXTERNAL,
                0,  # NumberOfAuxSymbols
            )

        header = COFF_HEADER.pack(
            self._machine,
            len(self._sections),
            0,  # TimeDateStamp
            raw_data_end + len(relocation_tables),  # PointerToSymbolTable
            len(self._symbols),
            0,  # SizeOfOptionalHeader
            0,  # Characteristics
        )
        # The string table declares its own size in its first four bytes, and offsets into it
        # count from the start of that size field.
        string_table = struct.pack("<I", 4 + len(self._string_table)) + bytes(self._string_table)
        return bytes(header + section_headers + raw_data + relocation_tables + symbol_table) + string_table

    def _section_offset(self, section_number: int) -> int:
        offset = COFF_HEADER.size + COFF_SECTION_HEADER.size * len(self._sections)
        for section in self._sections[: section_number - 1]:
            offset += len(section["data"])
        return offset

    def _intern(self, name: str) -> int:
        offset = 4 + len(self._string_table)
        self._string_table += name.encode() + b"\0"
        return offset

    def _section_name(self, name: str) -> bytes:
        if len(name) <= 8:
            return name.encode().ljust(8, b"\0")
        # A long section name is a slash followed by a decimal string table offset.
        return f"/{self._intern(name)}".encode().ljust(8, b"\0")

    def _symbol_name(self, name: str) -> bytes:
        if len(name) <= 8:
            return name.encode().ljust(8, b"\0")
        # A long symbol name is a zero dword followed by a string table offset.
        return struct.pack("<II", 0, self._intern(name))


def build_long_section_names() -> bytes:
    """
    Section names as a stock mingw-w64 GCC emits them: two too long for the eight-byte field and
    so held in the string table, one that fits. The symbols make both string table offsets valid
    symbol table indices as well, so reading an offset as an index yields a wrong name rather
    than an IndexError.
    """
    writer = CoffObjectWriter()
    writer.add_section(".rdata$zzz", b"\0" * 4)
    writer.add_section(".debug$S", b"\0" * 4)
    writer.add_section(".gcc_except_table", b"\0" * 4)
    for i in range(16):
        writer.add_symbol(f"_sym{i}", 0, 1)
    return writer.build()


def build_reloc_dir32() -> bytes:
    """
    A DIR32 field holding the addend -6, whose 32-bit pattern carries past the top of the field
    once the symbol's address is added.
    """
    writer = CoffObjectWriter()
    text = writer.add_section(".text", struct.pack("<i", -6) + b"\0" * 12)
    target = writer.add_symbol("_target", 8, text)
    writer.add_relocation(text, 0, target, IMAGE_REL_I386_DIR32)
    return writer.build()


def build_reloc_rel32() -> bytes:
    """
    A backwards `call rel32` at offset 4, whose displacement field at offset 5 holds the addend
    -4 and resolves to a negative displacement.
    """
    writer = CoffObjectWriter()
    text = writer.add_section(".text", b"\xc3" * 4 + b"\xe8" + struct.pack("<i", -4) + b"\x90" * 3)
    callee = writer.add_symbol("_callee", 0, text)
    writer.add_relocation(text, 5, callee, IMAGE_REL_I386_REL32)
    return writer.build()


def build_reloc_outside_section() -> bytes:
    """
    A relocation whose field lies past the end of the section that owns it, and so inside the
    next section's raw data. The two sections hold different fill bytes, so a loader that
    resolves the relocation against the whole file rewrites bytes of `.data` that no relocation
    names.
    """
    writer = CoffObjectWriter()
    text = writer.add_section(".text", b"\x90" * 16)
    writer.add_section(".data", b"\xaa" * 16)
    target = writer.add_symbol("_target", 8, text)
    # .text holds 16 bytes, so offset 0x10 is the first byte of .data.
    writer.add_relocation(text, 0x10, target, IMAGE_REL_I386_DIR32)
    return writer.build()


def build_reloc_outside_file() -> bytes:
    """
    A relocation whose field lies past the end of the whole object, not merely past its section.
    A loader that adds the offset to the section's PointerToRawData without checking it reads and
    writes an address that nothing maps.
    """
    writer = CoffObjectWriter()
    text = writer.add_section(".text", b"\x90" * 16)
    target = writer.add_symbol("_target", 8, text)
    writer.add_relocation(text, 0x4000000, target, IMAGE_REL_I386_DIR32)
    return writer.build()


def build_reloc_arm64() -> bytes:
    """
    One instance of each ARM64 relocation the backend implements. `target` and `aligned` are
    defined here, `ext_fn` is undefined so that the loader has to route the branch through an
    extern stub. Several fields hold a non-zero addend, which the relocation has to preserve.
    """
    writer = CoffObjectWriter(IMAGE_FILE_MACHINE_ARM64)
    text = writer.add_section(
        ".text",
        struct.pack(
            "<11L",
            0x94000000,  # bl ext_fn
            0x90000000,  # adrp x0, target
            0x91000000,  # add x0, x0, :lo12:target
            0xB9400001,  # ldr w1, [x0, :lo12:target]
            0x91001000,  # add x0, x0, :lo12:target, addend 4
            0x3DC00001,  # ldr q1, [x0, :lo12:aligned]
            0xD65F03C0,  # ret
            0,  # padding
            8,  # 64-bit address slot, addend 8
            0,
            4,  # 32-bit RVA slot, addend 4
        ),
    )
    # `aligned` is 16-byte aligned, so that the vector load above can reach it.
    target = writer.add_symbol("target", 0, text, IMAGE_SYM_TYPE_FUNCTION)
    ext_fn = writer.add_symbol("ext_fn", 0, 0, IMAGE_SYM_TYPE_FUNCTION)
    aligned = writer.add_symbol("aligned", 4, text)

    writer.add_relocation(text, 0x00, ext_fn, IMAGE_REL_ARM64_BRANCH26)
    writer.add_relocation(text, 0x04, target, IMAGE_REL_ARM64_PAGEBASE_REL21)
    writer.add_relocation(text, 0x08, target, IMAGE_REL_ARM64_PAGEOFFSET_12A)
    writer.add_relocation(text, 0x0C, target, IMAGE_REL_ARM64_PAGEOFFSET_12L)
    writer.add_relocation(text, 0x10, target, IMAGE_REL_ARM64_PAGEOFFSET_12A)
    writer.add_relocation(text, 0x14, aligned, IMAGE_REL_ARM64_PAGEOFFSET_12L)
    writer.add_relocation(text, 0x20, target, IMAGE_REL_ARM64_ADDR64)
    writer.add_relocation(text, 0x28, target, IMAGE_REL_ARM64_ADDR32NB)
    return writer.build()


def build_reloc_armnt() -> bytes:
    """
    The Thumb-2 relocations the backend implements. `ext_ptr` is named only by the pointer slot,
    so nothing but its symbol type can decide that it is Thumb code.
    """
    writer = CoffObjectWriter(IMAGE_FILE_MACHINE_ARMNT)
    text = writer.add_section(
        ".text",
        struct.pack(
            "<14H",
            0xF240,
            0x0000,  # movw r0, #:lower16:thumbfn
            0xF2C0,
            0x0000,  # movt r0, #:upper16:thumbfn
            0xF000,
            0xF800,  # bl ext_fn
            0xF240,
            0x0104,  # movw r1, #:lower16:thumbfn, addend 4
            0xF2C0,
            0x0100,  # movt r1, #:upper16:thumbfn
            0x4770,  # bx lr
            0x0000,  # padding
            0x0000,
            0x0000,  # 32-bit pointer slot
        ),
    )
    # MSVC types a referenced but undefined function this way; clang leaves it untyped.
    thumbfn = writer.add_symbol("thumbfn", 0, text, IMAGE_SYM_TYPE_FUNCTION)
    ext_fn = writer.add_symbol("ext_fn", 0, 0, IMAGE_SYM_TYPE_FUNCTION)
    ext_ptr = writer.add_symbol("ext_ptr", 0, 0, IMAGE_SYM_TYPE_FUNCTION)

    writer.add_relocation(text, 0x00, thumbfn, IMAGE_REL_ARM_MOV32T)
    writer.add_relocation(text, 0x08, ext_fn, IMAGE_REL_ARM_BRANCH24T)
    writer.add_relocation(text, 0x0C, thumbfn, IMAGE_REL_ARM_MOV32T)
    writer.add_relocation(text, 0x18, ext_ptr, IMAGE_REL_ARM_ADDR32)
    return writer.build()


def build_unsupported_machine() -> bytes:
    """
    A header naming a machine type the COFF backend does not support, and nothing else.
    """
    return CoffObjectWriter(IMAGE_FILE_MACHINE_R4000).build()


OBJECTS = {
    "x86/coff_long_section_names.obj": build_long_section_names,
    "x86/coff_reloc_dir32.obj": build_reloc_dir32,
    "x86/coff_reloc_rel32.obj": build_reloc_rel32,
    "x86/coff_reloc_outside_section.obj": build_reloc_outside_section,
    "x86/coff_reloc_outside_file.obj": build_reloc_outside_file,
    "aarch64/coff_reloc_arm64.obj": build_reloc_arm64,
    "armel/coff_reloc_armnt.obj": build_reloc_armnt,
    "mips/coff_r4000.obj": build_unsupported_machine,
}


def main() -> None:
    for name, build in OBJECTS.items():
        path = os.path.normpath(os.path.join(TESTS_DIR, name))
        with open(path, "wb") as f:
            f.write(build())
        print(f"wrote {path}")


if __name__ == "__main__":
    main()
