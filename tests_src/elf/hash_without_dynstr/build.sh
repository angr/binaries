#!/usr/bin/env bash
# Rebuild the dangling-sh_link ELF fixture.
#
# Freestanding: no libc, no start files, so this needs only clang and GNU ld.
# The linker script discards .dynstr while keeping .hash and .dynsym, so the
# linker leaves sh_link = 0 in both -- the same section table u-boot images
# carry. --emit-relocs keeps the link-time .rel sections around as u-boot does.
# The build is deterministic; rebuilding produces the same bytes.
set -euo pipefail
cd "$(dirname "$0")"
out=../../../tests/i386/hash_without_dynstr
clang --target=i386-unknown-linux-gnu -c -fPIC -O1 -ffreestanding \
    -fno-asynchronous-unwind-tables -o table.o table.c
ld -m elf_i386 -pie --emit-relocs -T table.lds -o "$out" table.o
rm -f table.o
