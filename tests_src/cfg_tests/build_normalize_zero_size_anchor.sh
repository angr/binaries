#!/bin/bash
# Builds tests/armel/normalize_zero_size_anchor.
#
# Any ARM EABI cross gcc will do; the committed binary was produced with GCC
# 15.3.0 from nixpkgs, which you can get with:
#
#   nix shell nixpkgs#pkgsCross.armv7l-hf-multiplatform.buildPackages.gcc
#
# CC defaults to that toolchain's driver name.  Assembling and linking are kept
# separate so the temporary object name does not leak into the symbol table.
set -ex

BASE_DIR=$(dirname "$0")
OUTPUT_DIR=$BASE_DIR/../../tests
CC=${CC:-armv7l-unknown-linux-gnueabihf-gcc}

"$CC" -march=armv7-a -marm -c \
    -o "$BASE_DIR/normalize_zero_size_anchor.o" \
    "$BASE_DIR/normalize_zero_size_anchor.s"
"$CC" -nostdlib -static -Wl,--build-id=none \
    -o "$OUTPUT_DIR/armel/normalize_zero_size_anchor" \
    "$BASE_DIR/normalize_zero_size_anchor.o"
rm -f "$BASE_DIR/normalize_zero_size_anchor.o"
