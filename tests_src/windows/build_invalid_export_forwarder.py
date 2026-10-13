"""Build the PE fixture whose forwarded-export string is not valid UTF-8.

The linker accepts an ASCII forwarder and produces a normal DLL. After linking,
this builder replaces the second byte of ``user32.MessageBoxA`` with 0x9f. The
result remains a structurally valid PE, and pefile exposes the exact malformed
string as the export's byte-valued ``forwarder`` field.

Run from this directory with the stated MinGW toolchain on ``PATH``:

    python3 -P build_invalid_export_forwarder.py

The compiler is x86_64-w64-mingw32-gcc 15.3.0 with binutils 2.46 and
mingw-w64 14.0.0. The fixture links no CRT or import library, so ``-nostdlib``
keeps the image independent of a particular MinGW threading runtime;
``--no-insert-timestamp`` makes repeated builds identical.
"""

from __future__ import annotations

import hashlib
from pathlib import Path
import subprocess
import tempfile


HERE = Path(__file__).resolve().parent
REPOSITORY = HERE.parents[1]
OUTPUT = REPOSITORY / "tests" / "x86_64" / "windows" / "invalid_export_forwarder.dll"
VALID_FORWARDER = b"user32.MessageBoxA"
INVALID_FORWARDER = b"u\x9fer32.MessageBoxA"


def main() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        linked = Path(tmpdir) / OUTPUT.name
        subprocess.run(
            [
                "x86_64-w64-mingw32-gcc",
                "-shared",
                "-nostdlib",
                "-Wl,--no-insert-timestamp",
                "-Wl,--image-base,0x180000000",
                "-o",
                str(linked),
                str(HERE / "invalid_export_forwarder.c"),
                str(HERE / "invalid_export_forwarder.def"),
            ],
            check=True,
        )
        data = linked.read_bytes()

    offset = data.find(VALID_FORWARDER)
    if offset < 0:
        raise RuntimeError("the linked image does not contain the expected forwarder")
    if len(VALID_FORWARDER) != len(INVALID_FORWARDER):
        raise RuntimeError("the mutation must not change the PE's layout")

    # GNU ld also records the forwarder spelling in the COFF string table. The
    # first occurrence is the export-directory string; mutate only that one so
    # the fixture differs from an ordinary compiler output in exactly one byte.
    OUTPUT.write_bytes(data[:offset] + INVALID_FORWARDER + data[offset + len(VALID_FORWARDER) :])
    print(f"{hashlib.sha256(OUTPUT.read_bytes()).hexdigest()}  {OUTPUT.relative_to(REPOSITORY)}")


if __name__ == "__main__":
    main()
