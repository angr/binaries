#!/bin/bash
# Builds the Mach-O MH_BUNDLE fixtures: a host executable and the bundles it
# loads with dlopen.
#
# plugin.bundle is linked with -bundle_loader, so its imports from the host are
# bound to the main executable. plugin_flat.bundle is linked with
# -undefined dynamic_lookup, as Python extension modules are, so the same
# imports are bound by flat lookup. The arm64 build targets macOS 12 and so uses
# chained fixups, the x86_64 build targets macOS 10.15 and so uses bind
# opcodes. ld64 falls back to bind opcodes for -undefined dynamic_lookup, which
# is why plugin_flat.bundle is only built for x86_64.
#
# Linking needs the macOS SDK. Built with Apple clang 21.0.0 (clang-2100.3.34.2).
set -ex

BASE_DIR=$(dirname $0)
OUTPUT_DIR=$BASE_DIR/../../tests

clang -target arm64-apple-macos12 -O1 $BASE_DIR/host.c -o $OUTPUT_DIR/aarch64/macho_bundle/host
clang -target arm64-apple-macos12 -O1 -bundle -bundle_loader $OUTPUT_DIR/aarch64/macho_bundle/host \
    $BASE_DIR/plugin.c -o $OUTPUT_DIR/aarch64/macho_bundle/plugin.bundle

clang -target x86_64-apple-macos10.15 -O1 $BASE_DIR/host.c -o $OUTPUT_DIR/x86_64/macho_bundle/host
clang -target x86_64-apple-macos10.15 -O1 -bundle -bundle_loader $OUTPUT_DIR/x86_64/macho_bundle/host \
    $BASE_DIR/plugin.c -o $OUTPUT_DIR/x86_64/macho_bundle/plugin.bundle
clang -target x86_64-apple-macos10.15 -O1 -bundle -undefined dynamic_lookup \
    $BASE_DIR/plugin.c -o $OUTPUT_DIR/x86_64/macho_bundle/plugin_flat.bundle
