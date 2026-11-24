#!/bin/sh
# Builds tests/x86_64/computed_goto_stateless_labels: stripped -O2 PIE, .eh_frame kept (GCC default), no CET.
# Run from the repository root, e.g. inside `docker run --rm --platform linux/amd64 -v "$PWD":/w -w /w gcc:16.2.0-trixie`.
set -e
gcc -O2 -falign-labels=16 -fPIE -pie -fcf-protection=none -o tests/x86_64/computed_goto_stateless_labels tests_src/computed_goto_stateless_labels.c
strip tests/x86_64/computed_goto_stateless_labels
