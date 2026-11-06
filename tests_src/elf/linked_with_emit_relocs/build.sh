#!/usr/bin/env bash
# Rebuild the already-linked-image relocation fixture.
#
# Freestanding: no libc, no start files, so this needs only clang and GNU ld.
# The section table is ordinary; what the fixture carries is a fixed link
# address of 0x1000 and, through --emit-relocs, the link-time .rel.text and
# .rel.data the linker has already applied. The build is deterministic;
# rebuilding produces the same bytes.
set -euo pipefail
cd "$(dirname "$0")"
out=../../../tests/i386/linked_with_emit_relocs
clang --target=i386-unknown-linux-gnu -c -fPIC -O1 -ffreestanding \
    -fno-asynchronous-unwind-tables -o table.o table.c
ld -m elf_i386 -pie --emit-relocs -T table.lds -o "$out" table.o
rm -f table.o
