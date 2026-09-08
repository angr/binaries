#!/usr/bin/env bash
# Build tests/x86_64/efi_short_data_directory.efi, a UEFI application whose PE
# optional header declares only six data directory entries
# (NumberOfRvaAndSizes == 6, SizeOfOptionalHeader == 160).
#
# No linker emits that. GNU ld, LLD, tcc, Zig, Go, Open Watcom's wlink and
# EDK2's GenFw were all measured writing 16, and neither ld nor lld-link has a
# flag for it. Real EFI stub images get a short directory by writing the header
# in assembly instead, and letting the assembler count the entries: see the
# Linux kernel's arch/x86/boot/header.S and arch/arm64/kernel/efi-header.S.
# efi_header.S does the same, with
#
#     .long (section_table - .) / 8    /* NumberOfRvaAndSizes */
#
# so the field follows from how many directory entries the source declares
# rather than being asserted. Everything else -- section sizes, addresses,
# SizeOfImage, SizeOfHeaders -- is computed by ld from efi.lds.
#
# Usage: ./build.sh
set -euo pipefail

cd "$(dirname "$0")"

# Pinned to the same nixpkgs revision as the angr development environment.
NIXPKGS="github:NixOS/nixpkgs/6b5e5b7a6631f065bf6908986990b37d845f847f"
OUT="../../tests/x86_64/efi_short_data_directory.efi"

nix shell "$NIXPKGS#gcc" "$NIXPKGS#binutils" --command bash -euo pipefail -c '
	gcc -c -m64 -ffreestanding -fno-stack-protector \
	    -fno-asynchronous-unwind-tables -fno-pie -mno-red-zone -Os \
	    -o efi_main.o efi_main.c
	gcc -c -o efi_header.o efi_header.S
	ld -nostdlib -static --no-dynamic-linker -T efi.lds \
	    -o efi_stub.elf efi_header.o efi_main.o
	objcopy -O binary efi_stub.elf "$1"
	rm -f efi_main.o efi_header.o efi_stub.elf
' -- "$OUT"

# Read the field back out of the built image rather than trusting the source.
python3 - "$OUT" <<'PY'
import struct, sys

data = open(sys.argv[1], "rb").read()
assert data[:2] == b"MZ", "not a DOS image"
pe = struct.unpack_from("<I", data, 0x3C)[0]
assert data[pe : pe + 4] == b"PE\0\0", "no PE signature"
optional = pe + 24
magic = struct.unpack_from("<H", data, optional)[0]
count = struct.unpack_from("<I", data, optional + (108 if magic == 0x20B else 92))[0]
size = struct.unpack_from("<H", data, pe + 20)[0]
print(f"{sys.argv[1]}: magic={magic:#05x} SizeOfOptionalHeader={size} NumberOfRvaAndSizes={count}")
assert count == 6, f"expected 6 data directories, got {count}"
assert size == 160, f"expected SizeOfOptionalHeader 160, got {size}"
PY
