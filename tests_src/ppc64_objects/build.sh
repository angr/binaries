#!/bin/sh
# Rebuilds tests/ppc64/empty_object.o and tests/ppc64/simple_object.o.
# Run from this directory. clang 21.1.8 reproduces both byte for byte; it needs
# no PowerPC sysroot, because neither source includes a header.
set -eu
for name in empty_object simple_object; do
    clang -target powerpc64-unknown-linux-gnu -c -O1 \
        -o "../../tests/ppc64/$name.o" "$name.c"
done
