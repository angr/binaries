#!/bin/sh
# Rebuild the MIPS n32 and O64 fixtures.
#
# Both ABIs put a 64-bit MIPS instruction stream in an ELFCLASS32 container, which is what
# these fixtures exist to cover: the prologues spill $gp with a 64-bit `sd`, an instruction a
# 32-bit MIPS decoder cannot produce, while every ELF structure in the file stays Elf32_*.
#
# The toolchain is nixpkgs' mips64el n32 cross compiler, which is prebuilt in the binary cache.
set -eux
D=$(dirname "$(readlink -f "$0")")
cd "$D"

CROSS='nixpkgs#pkgsCross.mips64el-linux-gnuabin32.buildPackages'
GCC="nix shell $CROSS.gcc --command mips64el-unknown-linux-gnuabin32-gcc"
LD="nix shell $CROSS.binutils --command mips64el-unknown-linux-gnuabin32-ld.bfd"

# n32. -static against glibc is not possible for this target (nixpkgs ships no static libc
# for it), so the static executables are -nostdlib with a hand-written _start.
$GCC -Os -c -o n32_el.o mipsn32_fixture.c
$GCC -Os -EB -static -nostdlib -Wl,-e,_start -o n32_be_static mipsn32_fixture.c start.c
$GCC -Os -o n32_el_dynamic mipsn32_fixture.c

# O64. The gcc driver's default emulation is n32-only: given -mabi=o64 it reports an
# incompatible ABI, trips a BFD assertion, and segfaults ld (binutils 2.46). Compile with
# gcc and link by hand against the o32/o64 emulation instead.
$GCC -Os -mabi=o64 -c -o o64_el.o mipsn32_fixture.c
$GCC -Os -mabi=o64 -c -o o64_start.o start.c
$LD -m elf32ltsmip -e _start -o o64_el_static o64_el.o o64_start.o
rm -f o64_start.o
