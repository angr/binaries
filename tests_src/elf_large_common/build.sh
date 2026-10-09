#!/bin/sh
# Build tests/x86_64/large_common.o.
#
# The point of the fixture is the st_shndx of big_buffer: SHN_X86_64_LCOMMON
# (0xff02), which readelf prints as "LARGE_COM". Only -mcmodel=medium puts a
# symbol there, and only -fcommon leaves it a common symbol at all, since gcc 10
# defaults to -fno-common.
#
#   nix shell nixpkgs#gcc --command sh build.sh
#
# Verify with:
#   readelf -sW ../../tests/x86_64/large_common.o | grep big_buffer
# which must show Ndx LARGE_COM.
set -eu

cd "$(dirname "$0")"

gcc -mcmodel=medium -fcommon -mlarge-data-threshold=65536 \
    -c large_common.c -o ../../tests/x86_64/large_common.o

readelf -sW ../../tests/x86_64/large_common.o | grep big_buffer
