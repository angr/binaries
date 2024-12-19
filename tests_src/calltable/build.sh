#!/bin/bash
set -ex

BASE_DIR=$(dirname $0)
OUTPUT_DIR=$BASE_DIR/../../tests

binaries="resolver_with_check_tailcall resolver_with_check resolver_without_check_tailcall resolver_without_check"

for binary in $binaries; do
    for opt_level in 0 1 2; do
        gcc -o $OUTPUT_DIR/x86_64/calltable/$binary-o$opt_level $BASE_DIR/$binary.c -O$opt_level
        gcc -o $OUTPUT_DIR/x86/calltable/$binary-o$opt_level $BASE_DIR/$binary.c -O$opt_level -m32 -fno-pie
    done
done
