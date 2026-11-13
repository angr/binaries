#!/usr/bin/env bash
# Build the compiler- and linker-produced Mach-O fixtures used by cle's filetype tests.

set -euo pipefail

base_dir=$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")" && pwd)
output_dir=$base_dir/../../tests/x86_64

clang=${CLANG:-clang}
ld64=${LD64:-ld64.lld}
trap 'rm -f "$base_dir/loader.o" "$base_dir/fixture.o" "$base_dir/bundle_loader"' EXIT

cd "$base_dir"

common_ld=(
    -arch x86_64
    -platform_version macos 11.0 11.0
    -no_adhoc_codesign
    -no_uuid
    -oso_prefix "$base_dir/"
)

common_cc=(
    -target x86_64-apple-macos11
    -O1
    -g
    -ffreestanding
    -nostdinc
    -fdebug-compilation-dir=.
    "-fdebug-prefix-map=$base_dir=."
)

"$clang" "${common_cc[@]}" -c loader.c -o loader.o
"$clang" "${common_cc[@]}" -c fixture.c -o fixture.o

"$ld64" "${common_ld[@]}" -e _main -exported_symbol _host_value -no_pie \
    loader.o -o bundle_loader
"$ld64" "${common_ld[@]}" -bundle -bundle_loader bundle_loader -U _flat_value \
    fixture.o -o "$output_dir/macho_bundle"
