#!/usr/bin/env python3
"""
Builds tests/ppc64/fauxware_notype_import, a copy of tests/ppc64/fauxware whose
"puts" and "exit" imports are declared STT_NOTYPE instead of STT_FUNC.

An object file may leave an imported function untyped, and plenty do: undefined
symbols carry no type unless a header declared one. On PowerPC64 ELFv1 that
matters to a loader, because a jump slot there holds a whole function descriptor
rather than an address, and how much space an unresolved import is given depends
on what the symbol table says it is. Nothing in this repository had the shape --
every ppc64 fixture types its imported functions, and the four that do carry an
untyped jump-slot import name __gmon_start__, which is weak and so never
resolved at all.

Only the two type nibbles change; the binding stays STB_GLOBAL and every other
byte is the original's, so the file still describes the same program.
"""

from __future__ import annotations

import os
import shutil

from elftools.elf.elffile import ELFFile

HERE = os.path.dirname(os.path.realpath(__file__))
TESTS = os.path.join(HERE, "..", "..", "tests", "ppc64")
SOURCE = os.path.join(TESTS, "fauxware")
OUTPUT = os.path.join(TESTS, "fauxware_notype_import")

# "puts" sits in the middle of the imports and "exit" is the last one, so the pair covers a
# descriptor read that stays inside the extern object and one that runs off its end.
UNTYPED = ("puts", "exit")

STT_NOTYPE = 0


def st_info_offsets(path: str, names: tuple[str, ...]) -> dict[str, int]:
    """File offset of each named dynamic symbol's st_info byte."""
    with open(path, "rb") as stream:
        dynsym = ELFFile(stream).get_section_by_name(".dynsym")
        if dynsym is None:
            raise SystemExit(f"{path} has no .dynsym")
        found = {}
        for index, symbol in enumerate(dynsym.iter_symbols()):
            if symbol.name in names:
                # Elf64_Sym is st_name(4) st_info(1) st_other(1) st_shndx(2) st_value(8) st_size(8)
                found[symbol.name] = dynsym["sh_offset"] + index * dynsym["sh_entsize"] + 4
    missing = set(names) - set(found)
    if missing:
        raise SystemExit(f"{path} has no dynamic symbol named {', '.join(sorted(missing))}")
    return found


def main() -> None:
    shutil.copyfile(SOURCE, OUTPUT)
    offsets = st_info_offsets(SOURCE, UNTYPED)
    with open(OUTPUT, "r+b") as stream:
        for name, offset in sorted(offsets.items()):
            stream.seek(offset)
            info = stream.read(1)[0]
            stream.seek(offset)
            stream.write(bytes([(info & 0xF0) | STT_NOTYPE]))
            print(f"{name}: st_info {info:#04x} -> {(info & 0xF0) | STT_NOTYPE:#04x}")
    print(f"wrote {OUTPUT}")


if __name__ == "__main__":
    main()
