#!/usr/bin/env bash
# Reproducible build of the AArch64 LSE / RCpc fixture.
#
#   nix shell github:NixOS/nixpkgs/6b5e5b7a6631f065bf6908986990b37d845f847f#pkgsCross.aarch64-multiplatform.buildPackages.gcc \
#       --command ./build.sh <builddir>
#
# Everything the build reads is either in ./src or in the nix store; nothing
# from the invoking environment reaches the artifact.
set -euo pipefail

BUILD_DIR=${1:?usage: build.sh <builddir>}
SRC_DIR=$(cd "$(dirname "$0")/src" && pwd)

export SOURCE_DATE_EPOCH=0
export LC_ALL=C
export TZ=UTC
export NIX_HARDENING_ENABLE=""      # no -fPIE / -fstack-protector from the wrapper

CC=aarch64-unknown-linux-gnu-gcc
STRIP=aarch64-unknown-linux-gnu-strip

COMMON="-O2 -mno-outline-atomics -std=gnu11 -Wall
        -ffreestanding -fno-stack-protector -fno-asynchronous-unwind-tables
        -fno-unwind-tables -fno-ident
        -ffile-prefix-map=$BUILD_DIR=. -ffile-prefix-map=$SRC_DIR=.
        -fdebug-prefix-map=$BUILD_DIR=. -fdebug-prefix-map=$SRC_DIR=."

rm -rf "$BUILD_DIR"
mkdir -p "$BUILD_DIR"
cp "$SRC_DIR"/*.c "$SRC_DIR"/*.h "$BUILD_DIR"/
cd "$BUILD_DIR"

$CC $COMMON -march=armv8.1-a -c -o lse.o      lse.c
$CC $COMMON -march=armv8.1-a -c -o lse_asm.o  lse_asm.c
$CC $COMMON -march=armv8.3-a -c -o rcpc.o     rcpc.c
$CC $COMMON -march=armv8.4-a -c -o rcpc2.o    rcpc2.c
$CC $COMMON -march=armv8.1-a -c -o start.o    start.c

$CC -nostdlib -nostartfiles -static -no-pie \
    -Wl,--build-id=none -Wl,-z,noexecstack -Wl,-e,_start \
    -o lse_atomics lse.o lse_asm.o rcpc.o rcpc2.o start.o

$STRIP --strip-debug --remove-section=.comment --remove-section=.note.GNU-stack \
       lse_atomics

sha256sum lse_atomics
