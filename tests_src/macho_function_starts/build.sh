#!/bin/bash
# Builds tests/aarch64/fcadd_function_starts.macho.
#
# Assembling and linking need no Apple SDK. ld64.lld signs the output ad hoc and
# uses its file name as the signing identifier, so the output keeps this name.
#
# Built with clang 21.1.8 and LLD 21.1.8 from nixpkgs (llvmPackages_21.clang-unwrapped
# and lld_21); CLANG and LD64 override the tool names.
set -ex

BASE_DIR=$(dirname $0)
OUTPUT=$BASE_DIR/../../tests/aarch64/fcadd_function_starts.macho
OBJECT=$(mktemp)
trap 'rm -f "$OBJECT"' EXIT

${CLANG:-clang} -target arm64-apple-macos11 -march=armv8.3-a -c \
    $BASE_DIR/fcadd_function_starts.s -o "$OBJECT"
${LD64:-ld64.lld} -arch arm64 -platform_version macos 11.0 11.0 -e _start -x \
    -o $OUTPUT "$OBJECT"
