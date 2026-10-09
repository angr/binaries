#!/usr/bin/env bash
# Rebuild tests/i386/bios.bin.truncated.elf.
#
# bios.bin.elf is a BIOS image whose only PT_LOAD starts at file offset 0x70, so
# the ELF header is not mapped anywhere. Cutting the file at the end of that
# segment's file bytes -- 0x70 + 0x63f0 -- keeps everything the program headers
# call loadable and drops the section header table, which starts at 0x6474.
# pyelftools then cannot walk the sections, which is the state CLE's
# no-section-headers fallback exists for.
set -euo pipefail
cd "$(dirname "$0")"
head -c $((0x70 + 0x63f0)) ../../tests/i386/bios.bin.elf > ../../tests/i386/bios.bin.truncated.elf
