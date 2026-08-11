#!/usr/bin/env python3
"""
Builds the TE images in tests/i386 and tests/aarch64 that cle's TE loader tests use.

A TE image is a PE image whose DOS and PE headers have been replaced by a 40-byte TE header. The
section table is copied verbatim, so its file offsets still describe the original PE file. This
repository carries no UEFI firmware to strip one out of, so the images are assembled by hand.
"""

from __future__ import annotations

import os
import struct

TE_HEADER = struct.Struct("<HHBBHIIQIIII")
SECTION_HEADER = struct.Struct("<8sIIIIIIHHI")

TE_SIGNATURE = 0x5A56  # b"VZ"
IMAGE_FILE_MACHINE_I386 = 0x014C
IMAGE_FILE_MACHINE_ARM64 = 0xAA64
EFI_APPLICATION = 10

IMAGE_SCN_CNT_CODE = 0x00000020
IMAGE_SCN_CNT_INITIALIZED_DATA = 0x00000040
IMAGE_SCN_CNT_UNINITIALIZED_DATA = 0x00000080
IMAGE_SCN_MEM_EXECUTE = 0x20000000
IMAGE_SCN_MEM_READ = 0x40000000
IMAGE_SCN_MEM_WRITE = 0x80000000

IMAGE_BASE = 0x400000
ENTRY_RVA = 0x1004
# kept larger than the TE header so that file offsets need the strip adjustment; with the two equal
# every address comes out right without it
STRIPPED_SIZE = 0x100

# nop; xor eax, eax; ret
I386_TEXT = b"\x90" * 4 + b"\x31\xc0\xc3" + b"\xcc" * 9
# nop; mov w0, #0; ret
AARCH64_TEXT = b"\x1f\x20\x03\xd5" + b"\x00\x00\x80\x52" + b"\xc0\x03\x5f\xd6" + b"\x1f\x20\x03\xd5"

OUT_DIR = os.path.join(os.path.dirname(os.path.realpath(__file__)), "..", "..", "tests")


def build_te(machine: int, text: bytes) -> bytes:
    """
    Assemble a TE image holding an executable .text with the entry point 4 bytes into it, a .data
    whose virtual size runs past its raw data, and a .bss with no raw data at all.
    """
    sections = [
        # (name, virtual address, virtual size, characteristics, raw data)
        (".text", 0x1000, len(text), IMAGE_SCN_CNT_CODE | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_EXECUTE, text),
        (
            ".data",
            0x2000,
            0x18,
            IMAGE_SCN_CNT_INITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE,
            b"\xaa" * 0x10,
        ),
        (".bss", 0x3000, 0x20, IMAGE_SCN_CNT_UNINITIALIZED_DATA | IMAGE_SCN_MEM_READ | IMAGE_SCN_MEM_WRITE, b""),
    ]

    stripped_offset = STRIPPED_SIZE - TE_HEADER.size
    body_offset = TE_HEADER.size + len(sections) * SECTION_HEADER.size

    section_headers = b""
    body = b""
    for name, virtual_address, virtual_size, characteristics, raw_data in sections:
        section_headers += SECTION_HEADER.pack(
            name.encode().ljust(8, b"\0"),
            virtual_size,
            virtual_address,
            len(raw_data),
            # a recorded file offset counts from the start of the pre-strip PE file
            body_offset + len(body) + stripped_offset,
            0,  # PointerToRelocations
            0,  # PointerToLinenumbers
            0,  # NumberOfRelocations
            0,  # NumberOfLinenumbers
            characteristics,
        )
        body += raw_data

    header = TE_HEADER.pack(
        TE_SIGNATURE,
        machine,
        len(sections),
        EFI_APPLICATION,
        STRIPPED_SIZE,
        ENTRY_RVA,
        sections[0][1],  # BaseOfCode
        IMAGE_BASE,
        0,  # DataDirectory[0].VirtualAddress
        0,  # DataDirectory[0].Size
        0,  # DataDirectory[1].VirtualAddress
        0,  # DataDirectory[1].Size
    )
    return header + section_headers + body


IMAGES = {
    os.path.join("i386", "te_sections.te"): lambda: build_te(IMAGE_FILE_MACHINE_I386, I386_TEXT),
    os.path.join("aarch64", "te_sections.te"): lambda: build_te(IMAGE_FILE_MACHINE_ARM64, AARCH64_TEXT),
}


def main() -> None:
    for name, build in IMAGES.items():
        path = os.path.normpath(os.path.join(OUT_DIR, name))
        with open(path, "wb") as f:
            f.write(build())
        print(f"wrote {path}")


if __name__ == "__main__":
    main()
