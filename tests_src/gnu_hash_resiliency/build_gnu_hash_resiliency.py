#!/usr/bin/env python3
"""
Builds tests/x86_64/gnu_hash_resiliency_0, a real binary with its hash tables zeroed out.

Some stripping and obfuscation tools blank the DT_HASH and DT_GNU_HASH tables of a finished
executable. They are only lookup accelerators, so the file keeps working -- its DT_SYMTAB,
DT_STRTAB, relocations and version tables are untouched -- but a .gnu.hash header of all zeroes
declares nbuckets == 0, and a reader that sizes the dynamic symbol table from the bucket array has
nothing to go on. pyelftools raises ValueError out of DynamicSegment.num_symbols() on that shape.

No linker emits an empty bucket array. GNU ld and LLD both special-case a table with nothing to
hash and write nbuckets = 1, so this cannot be produced by building something; it has to be a real
linker's output with the corruption applied. Hence this script rather than a source file: it takes
tests/x86_64/test_killing_ref, which carries both hash tables directly ahead of its symbol table,
and zeroes the bytes they occupy. Nothing else about the file changes, and running it again on the
same input reproduces the fixture byte for byte.
"""

from __future__ import annotations

import os

from elftools.elf.dynamic import DynamicSegment
from elftools.elf.elffile import ELFFile

HERE = os.path.dirname(os.path.realpath(__file__))
TESTS = os.path.join(HERE, "..", "..", "tests")
SOURCE = os.path.join(TESTS, "x86_64", "test_killing_ref")
TARGET = os.path.join(TESTS, "x86_64", "gnu_hash_resiliency_0")


def hash_table_extent(path: str) -> tuple[int, int]:
    """
    The file offsets spanned by DT_HASH and DT_GNU_HASH, which run up to DT_SYMTAB.
    """
    with open(path, "rb") as stream:
        elf = ELFFile(stream)
        segment = next(s for s in elf.iter_segments() if isinstance(s, DynamicSegment))
        offsets = []
        for tag in ("DT_HASH", "DT_GNU_HASH"):
            _, offset = segment.get_table_offset(tag)
            if offset is None:
                raise ValueError(f"{path} has no {tag}")
            offsets.append(offset)
        _, symtab = segment.get_table_offset("DT_SYMTAB")
        if symtab is None or symtab <= max(offsets):
            raise ValueError(f"{path} does not keep its hash tables directly ahead of DT_SYMTAB")
        return min(offsets), symtab


def main() -> None:
    start, end = hash_table_extent(SOURCE)
    with open(SOURCE, "rb") as stream:
        data = bytearray(stream.read())
    data[start:end] = bytes(end - start)
    with open(TARGET, "wb") as stream:
        stream.write(data)
    os.chmod(TARGET, os.stat(SOURCE).st_mode)
    print(f"zeroed {end - start} bytes at {start:#x} of {os.path.basename(SOURCE)}")
    print(f"wrote {TARGET}")


if __name__ == "__main__":
    main()
