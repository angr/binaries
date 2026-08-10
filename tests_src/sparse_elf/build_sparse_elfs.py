#!/usr/bin/env python3
"""
Builds the sparse i386 ELF images in tests/i386 that cle's rebase tests use.

Both images hold two PT_LOADs about 4 GB apart, leaving a sub-granule gap below the image and even
less room above it. No toolchain in this repository produces a layout that extreme, so they are
assembled by hand.
"""

from __future__ import annotations

import os
import struct

ELF_HEADER = struct.Struct("<16sHHIIIIIHHHHHH")
PROGRAM_HEADER = struct.Struct("<IIIIIIII")

ET_EXEC = 2
EM_386 = 3
PT_LOAD = 1
PF_R_X = 5

OUT_DIR = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", "..", "tests", "i386")

# a page and change below 0x10000, then a segment just under the top of the address space
LOW_LOAD = (0xF800, bytes(0x7F5))
HIGH_LOAD = (0xFFF00000, bytes(0x1000))


def build_sparse_elf(loads: list[tuple[int, bytes]]) -> bytes:
    """
    Assemble an ET_EXEC i386 ELF with one PT_LOAD per ``(vaddr, data)`` pair, in the order given,
    and no section headers.
    """
    offset = ELF_HEADER.size + PROGRAM_HEADER.size * len(loads)

    phdrs = b""
    body = b""
    for vaddr, data in loads:
        phdrs += PROGRAM_HEADER.pack(PT_LOAD, offset, vaddr, vaddr, len(data), len(data), PF_R_X, 0x1000)
        body += data
        offset += len(data)

    ehdr = ELF_HEADER.pack(
        b"\x7fELF\x01\x01\x01\x00" + bytes(8),
        ET_EXEC,
        EM_386,
        1,  # e_version
        loads[0][0],  # e_entry
        ELF_HEADER.size,  # e_phoff
        0,  # e_shoff
        0,  # e_flags
        ELF_HEADER.size,  # e_ehsize
        PROGRAM_HEADER.size,  # e_phentsize
        len(loads),  # e_phnum
        0,  # e_shentsize
        0,  # e_shnum
        0,  # e_shstrndx
    )
    return ehdr + phdrs + body


IMAGES = {
    "sparse_segments": lambda: build_sparse_elf([LOW_LOAD, HIGH_LOAD]),
    # program headers do not have to be sorted by vaddr, and cle keeps them in file order
    "sparse_segments_unsorted_phdrs": lambda: build_sparse_elf([HIGH_LOAD, LOW_LOAD]),
}


def main() -> None:
    for name, build in IMAGES.items():
        path = os.path.normpath(os.path.join(OUT_DIR, name))
        with open(path, "wb") as f:
            f.write(build())
        print(f"wrote {path}")


if __name__ == "__main__":
    main()
