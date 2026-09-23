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

# no_symbols.c holds no code, so the object has no symbols to list and clang
# writes no LC_SYMTAB at all. -g adds the four __DWARF accelerator tables;
# without it the object is 208 bytes holding the same empty __text. The other
# flags the two lines above pass are left off, having nothing to act on. Only
# x86_64 is built from it: for arm64 clang writes an LC_SYMTAB holding the local
# section symbol ltmp0, and an LC_DYSYMTAB beside it, so that object would not
# carry the shape this fixture is for.
clang -target x86_64-apple-macos11 -c -g \
    $BASE_DIR/no_symbols.c -o $OUTPUT_DIR/x86_64/relocatable_object_no_symtab.macho
