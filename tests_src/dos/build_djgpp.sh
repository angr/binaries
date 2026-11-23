#!/usr/bin/env bash
# Build the two DJGPP go32 fixtures from djgpp_hello.c. Reproducible: the
# toolchain is package djgpp_i586 (gcc 12.2.0 with DJGPP's binutils and libc)
# from the nixpkgs revision pinned in NIXPKGS below, not whatever is installed.
# The linker writes the go32 stub in front of the COFF image itself; no
# separate stubify step is needed.
#
#   djgpp_hello.exe           keeps the COFF symbol table. Debug info is
#                             stripped because the libgcc that gcc links in
#                             carries DWARF naming the toolchain's
#                             /nix/store path.
#   djgpp_hello_stripped.exe  the same executable with its symbol table
#                             stripped too, which is how go32 programs
#                             usually ship.

set -euo pipefail

NIXPKGS="github:NixOS/nixpkgs/6b5e5b7a6631f065bf6908986990b37d845f847f"
BASE_DIR=$(cd "$(dirname "$0")" && pwd)
OUTPUT_DIR=$BASE_DIR/../../tests/i386/dos
cd "$BASE_DIR"

nix shell "${NIXPKGS}#djgpp_i586" --command sh -c '
    set -e
    i586-pc-msdosdjgpp-gcc -O1 -o "$1/djgpp_hello.exe" djgpp_hello.c
    i586-pc-msdosdjgpp-strip --strip-debug "$1/djgpp_hello.exe"
    i586-pc-msdosdjgpp-strip -o "$1/djgpp_hello_stripped.exe" "$1/djgpp_hello.exe"
' sh "$OUTPUT_DIR"
