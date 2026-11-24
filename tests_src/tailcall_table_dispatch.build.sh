#!/bin/sh
# Builds tests/x86_64/tailcall_table_dispatch: stripped -O2 PIE, .eh_frame kept (GCC default), no CET.
# Run from the repository root, e.g. inside `docker run --rm --platform linux/amd64 -v "$PWD":/w -w /w gcc:16.2.0-trixie`.
set -e
gcc -O2 -fPIE -pie -fcf-protection=none -o tests/x86_64/tailcall_table_dispatch tests_src/tailcall_table_dispatch.c
strip tests/x86_64/tailcall_table_dispatch
