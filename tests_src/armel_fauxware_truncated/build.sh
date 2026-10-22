#!/usr/bin/env bash
# Rebuild tests/armel/fauxware.truncated.elf.
#
# tests/armel/fauxware has two PT_LOAD segments; the second one's file bytes end
# at 0xf04 + 0x140 = 0x1044, and the section header table starts at 0x11b0.
# Cutting the file at 0x1044 keeps every byte the program headers call loadable
# and leaves e_shoff past the end, so pyelftools cannot read even section 0.
# That is the state a truncated ELF arrives in, and CLE's no-section-headers
# fallback exists for it -- but PT_DYNAMIC and PT_GNU_RELRO are both still
# there, so the RELRO probe in MetaELF.__init__ reaches the section table first.
set -euo pipefail
cd "$(dirname "$0")"
head -c $((0xf04 + 0x140)) ../../tests/armel/fauxware > ../../tests/armel/fauxware.truncated.elf
