#!/usr/bin/env python3
"""
Clears p_filesz on the PT_DYNAMIC of a little-endian ELF64 object, leaving every other header alone.

The dynamic table itself, and the PT_LOAD that carries it, are untouched, so the table still has
contents in the file and at its load address; only PT_DYNAMIC stops saying how many bytes of it are
there. Loaders that trust p_filesz see an empty dynamic table, and loaders that do not have to find
the contents through the PT_LOAD instead.

No toolchain in this repository emits that combination and no object here has it, so tests that need
one have to be given it.
"""

from __future__ import annotations

import struct
import sys

ELF_MAGIC = b"\x7fELF"
ELFCLASS64 = 2
ELFDATA2LSB = 1
PT_DYNAMIC = 2

E_PHOFF = 0x20
E_PHENTSIZE = 0x36
E_PHNUM = 0x38
P_TYPE = 0
P_FILESZ = 32


def zero_dynamic_filesz(image: bytes) -> bytes:
    if image[:4] != ELF_MAGIC or image[4] != ELFCLASS64 or image[5] != ELFDATA2LSB:
        raise SystemExit("not a little-endian ELF64 object")

    out = bytearray(image)
    phoff = struct.unpack_from("<Q", out, E_PHOFF)[0]
    phentsize = struct.unpack_from("<H", out, E_PHENTSIZE)[0]
    phnum = struct.unpack_from("<H", out, E_PHNUM)[0]

    found = False
    for index in range(phnum):
        header = phoff + index * phentsize
        if struct.unpack_from("<I", out, header + P_TYPE)[0] == PT_DYNAMIC:
            struct.pack_into("<Q", out, header + P_FILESZ, 0)
            found = True

    if not found:
        raise SystemExit("no PT_DYNAMIC to clear")

    return bytes(out)


def main() -> None:
    if len(sys.argv) != 3:
        raise SystemExit(f"usage: {sys.argv[0]} INPUT OUTPUT")

    with open(sys.argv[1], "rb") as source:
        image = source.read()
    with open(sys.argv[2], "wb") as target:
        target.write(zero_dynamic_filesz(image))


if __name__ == "__main__":
    main()
