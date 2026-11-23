#!/bin/sh
# Rebuild the SPARC64 call fixture.
#
# The toolchain is clang and lld 21.1.8 from nixpkgs 6b5e5b7a6631f065bf6908986990b37d845f847f,
# both prebuilt in the binary cache. The fixture is freestanding: no libc, and _start
# never returns.
set -eux
D=$(dirname "$(readlink -f "$0")")
cd "$D"

NIXPKGS='github:NixOS/nixpkgs/6b5e5b7a6631f065bf6908986990b37d845f847f'
nix shell "$NIXPKGS#llvmPackages_21.clang-unwrapped" "$NIXPKGS#lld_21" --command sh -c '
    clang --target=sparcv9-unknown-linux-gnu -O2 -ffreestanding -fno-pic -fno-asynchronous-unwind-tables \
        -c -o calls.o calls.c
    ld.lld -static -e _start -o calls calls.o
    rm -f calls.o
'
sha256sum calls
