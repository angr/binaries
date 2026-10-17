#!/usr/bin/env bash
# Build the MIPS HI16/LO16 relocation fixtures from mips-hilo-rela.s.
#
# The assembler is GNU as 2.46 from the nixpkgs mips64el-linux-gnuabin32 cross
# toolchain, which covers all three ABIs from one binary. Run from this
# directory; the objects land beside the other MIPS fixtures.
#
#     nix shell nixpkgs#pkgsCross.mips64el-linux-gnuabin32.buildPackages.binutils
#     ./mips-hilo-generate.sh
#
# The -32 build warns "can't find matching low-part relocation for %hi
# operator"; that is the point of the fixture and the object is still emitted.
set -eu

AS=${AS:-mips64el-unknown-linux-gnuabin32-as}
TESTS=../../../tests

$AS -EB -64  -march=mips64r2 -mno-pdr -o "$TESTS/mips64/mips64-hilo-rela.o"  mips-hilo-rela.s
$AS -EB -n32 -march=mips64r2 -mno-pdr -o "$TESTS/mips64/mipsn32-hilo-rela.o" mips-hilo-rela.s
$AS -EB -32  -march=mips32r2 -mno-pdr -o "$TESTS/mips/mips-hilo-unpaired.o"  mips-hilo-rela.s
