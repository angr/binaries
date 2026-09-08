#!/bin/bash
# Builds tests/x86_64/x32_relocatable.o, an ELFCLASS32 container holding EM_X86_64
# code -- the x32 ABI, where the class gives the pointer width and the machine gives
# the instruction set.
#
# Compiling to an object needs no x32 libc, so any clang that can target it will do.
# Built with clang 21 from nixpkgs:
#     nix shell nixpkgs#clang --command ./build.sh
set -ex

BASE_DIR=$(dirname "$0")
OUTPUT_DIR=$BASE_DIR/../../tests/x86_64

clang -target x86_64-linux-gnux32 -O1 -c -nostdinc -ffreestanding \
    "$BASE_DIR/probe.c" -o "$OUTPUT_DIR/x32_relocatable.o"
