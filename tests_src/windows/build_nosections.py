"""Build the i386 PE fixtures whose section table maps nothing.

nosections.c is linked twice, at a 16-byte section and file alignment and at
lld-link's default alignment:

- ``lowalign.exe`` is the low-alignment image as linked. Every section's
  PointerToRawData equals its VirtualAddress.
- ``lowalign_nosections.exe`` is ``lowalign.exe`` with NumberOfSections set to
  zero: its code and import table are where ``lowalign.exe`` has them, but no
  section header describes them.
- ``nosections.exe`` is the default-alignment image with NumberOfSections set to
  zero, so no section maps its code.
- ``nosections_zeroheaders.exe`` is ``nosections.exe`` with SizeOfHeaders set to
  zero as well.
- ``emptysection.exe`` is the default-alignment image with its two section
  headers zeroed but for the first one's name, so the only section pefile reads
  is at RVA 0 with no size.

Run from this directory with clang, lld and llvm 21.1.8 from nixpkgs
6b5e5b7a6631f065bf6908986990b37d845f847f on ``PATH``:

    p=github:NixOS/nixpkgs/6b5e5b7a6631f065bf6908986990b37d845f847f
    nix shell $p#llvmPackages_21.clang-unwrapped $p#lld_21 $p#llvm_21 \\
        --command python3 -P build_nosections.py

``/brepro`` makes repeated builds identical. lld-link warns that ``/align``
was given without ``/driver`` and that the image may not run.
"""

from __future__ import annotations

import hashlib
from pathlib import Path
import struct
import subprocess
import tempfile


HERE = Path(__file__).resolve().parent
REPOSITORY = HERE.parents[1]
OUTPUT_DIR = REPOSITORY / "tests" / "i386" / "windows"
LINK_FLAGS = [
    "/nologo",
    "/brepro",
    "/entry:start",
    "/subsystem:console",
    "/nodefaultlib",
    "/fixed",
    "/safeseh:no",
    "/nxcompat:no",
    "/tsaware:no",
]


def without_section_table(image: bytes) -> bytes:
    """Return ``image`` with its COFF header's NumberOfSections set to zero."""
    e_lfanew = struct.unpack_from("<I", image, 0x3C)[0]
    if image[e_lfanew : e_lfanew + 4] != b"PE\0\0":
        raise RuntimeError("the linked image has no PE signature at e_lfanew")
    count = e_lfanew + 6
    return image[:count] + b"\0\0" + image[count + 2 :]


def with_zero_size_of_headers(image: bytes) -> bytes:
    """Return ``image`` with its PE32 optional header's SizeOfHeaders set to zero."""
    e_lfanew = struct.unpack_from("<I", image, 0x3C)[0]
    if image[e_lfanew : e_lfanew + 4] != b"PE\0\0" or struct.unpack_from("<H", image, e_lfanew + 24)[0] != 0x10B:
        raise RuntimeError("the linked image is not a PE32 image")
    size_of_headers = e_lfanew + 24 + 60
    return image[:size_of_headers] + bytes(4) + image[size_of_headers + 4 :]


def with_empty_sections(image: bytes) -> bytes:
    """Return ``image`` with every section header zeroed except the first one's name."""
    e_lfanew = struct.unpack_from("<I", image, 0x3C)[0]
    if image[e_lfanew : e_lfanew + 4] != b"PE\0\0":
        raise RuntimeError("the linked image has no PE signature at e_lfanew")
    count = struct.unpack_from("<H", image, e_lfanew + 6)[0]
    table = e_lfanew + 24 + struct.unpack_from("<H", image, e_lfanew + 20)[0]
    return image[: table + 8] + bytes(40 * count - 8) + image[table + 40 * count :]


def main() -> None:
    with tempfile.TemporaryDirectory() as tmpdir:
        tmp = Path(tmpdir)
        obj = tmp / "nosections.obj"
        lib = tmp / "kernel32.lib"
        subprocess.run(
            [
                "clang",
                "--target=i686-pc-windows-msvc",
                "-O1",
                "-ffreestanding",
                "-fno-builtin",
                "-fno-stack-protector",
                "-c",
                str(HERE / "nosections.c"),
                "-o",
                str(obj),
            ],
            check=True,
        )
        subprocess.run(
            ["llvm-dlltool", "-m", "i386", "-k", "-d", str(HERE / "nosections.def"), "-l", str(lib)],
            check=True,
        )

        def link(name: str, *flags: str) -> bytes:
            out = tmp / name
            subprocess.run(["lld-link", *LINK_FLAGS, *flags, f"/out:{out}", str(obj), str(lib)], check=True)
            return out.read_bytes()

        lowalign = link("lowalign.exe", "/align:16", "/filealign:16")
        default = link("default.exe")

    outputs = {
        "lowalign.exe": lowalign,
        "lowalign_nosections.exe": without_section_table(lowalign),
        "nosections.exe": without_section_table(default),
        "nosections_zeroheaders.exe": with_zero_size_of_headers(without_section_table(default)),
        "emptysection.exe": with_empty_sections(default),
    }
    for name, data in outputs.items():
        path = OUTPUT_DIR / name
        path.write_bytes(data)
        print(f"{hashlib.sha256(data).hexdigest()}  {path.relative_to(REPOSITORY)}")


if __name__ == "__main__":
    main()
