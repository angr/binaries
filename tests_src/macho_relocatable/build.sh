#!/bin/bash
# Builds the Mach-O MH_OBJECT (relocatable object) fixtures.
#
# Compiling to an object never resolves headers, so no Apple SDK is needed: any
# clang that can target Apple will do. The source is freestanding for that reason.
#
# Built with clang 21.1.8 from nixpkgs:
#     nix shell nixpkgs#clang --command ./build.sh
set -ex

BASE_DIR=$(dirname $0)
OUTPUT_DIR=$BASE_DIR/../../tests

clang -target arm64-apple-macos11  -O1 -c -nostdinc -ffreestanding \
    $BASE_DIR/shake_stub.c -o $OUTPUT_DIR/aarch64/relocatable_object.macho
clang -target x86_64-apple-macos11 -O1 -c -nostdinc -ffreestanding \
    $BASE_DIR/shake_stub.c -o $OUTPUT_DIR/x86_64/relocatable_object.macho
