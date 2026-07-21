#!/usr/bin/env python3
"""Build the synthetic 6502 overlapping-block regression fixture.

Run this script from the repository root.  The blob is loaded at 0x8000 by the
angr regression; its graph intentionally represents the code at 0x8005 as both
a three-byte block and a five-byte block.
"""

from pathlib import Path


CODE = bytes(
    (
        0xD0,
        0x03,  # 8000: bne 8005
        0x4C,
        0x05,
        0x80,  # 8002: jmp 8005
        0x18,  # 8005: clc
        0x69,
        0x03,  # 8006: adc #3
        0xD0,
        0xFB,  # 8008: bne 8005
        0x4C,
        0x0D,
        0x80,  # 800a: jmp 800d
        0x4C,
        0x0D,
        0x80,  # 800d: jmp 800d
    )
)


def main() -> None:
    output = Path("tests/6502/overlapping_blocks")
    output.write_bytes(CODE)


if __name__ == "__main__":
    main()
