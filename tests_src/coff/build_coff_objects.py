#!/usr/bin/env python3
"""
Builds the minimal i386 COFF objects in tests/x86 that cle's COFF loader tests use.

No object emitted by a real toolchain in this repository has a long section name or a relocation
field that wraps, so these are assembled by hand.
"""

from __future__ import annotations

import os
import struct

IMAGE_FILE_MACHINE_I386 = 0x14C
IMAGE_REL_I386_DIR32 = 0x0006
IMAGE_REL_I386_REL32 = 0x0014
IMAGE_SYM_CLASS_EXTERNAL = 2
IMAGE_SCN_TEXT = 0x60000020  # CNT_CODE | MEM_EXECUTE | MEM_READ

COFF_HEADER = struct.Struct("<HHLLLHH")
COFF_SECTION_HEADER = struct.Struct("<8sLLLLLLHHL")
COFF_SYMBOL = struct.Struct("<8sLhHBB")
COFF_RELOCATION = struct.Struct("<LLH")

OUT_DIR = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", "..", "tests", "x86")


class CoffObjectWriter:
    """
    Assembles a minimal i386 COFF object in memory.
    """

    def __init__(self):
        self._sections: list[dict] = []
        self._symbols: list[tuple[str, int, int]] = []
        self._string_table = bytearray()

    def add_section(self, name: str, data: bytes) -> int:
        """
        Append a section and return its one-based section number.
        """
        self._sections.append({"name": name, "data": data, "relocations": []})
        return len(self._sections)

    def add_symbol(self, name: str, value: int, section_number: int) -> int:
        """
        Append an external symbol defined at `value` bytes into `section_number`, and return its
        symbol table index.
        """
        self._symbols.append((name, value, section_number))
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
        for name, value, section_number in self._symbols:
            symbol_table += COFF_SYMBOL.pack(
                self._symbol_name(name),
                value,
                section_number,
                0,  # Type
                IMAGE_SYM_CLASS_EXTERNAL,
                0,  # NumberOfAuxSymbols
            )

        header = COFF_HEADER.pack(
            IMAGE_FILE_MACHINE_I386,
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


OBJECTS = {
    "coff_long_section_names.obj": build_long_section_names,
    "coff_reloc_dir32.obj": build_reloc_dir32,
    "coff_reloc_rel32.obj": build_reloc_rel32,
}


def main() -> None:
    for name, build in OBJECTS.items():
        path = os.path.normpath(os.path.join(OUT_DIR, name))
        with open(path, "wb") as f:
            f.write(build())
        print(f"wrote {path}")


if __name__ == "__main__":
    main()
