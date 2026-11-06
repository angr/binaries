#!/usr/bin/env bash
set -euo pipefail

cd -- "$(dirname -- "${BASH_SOURCE[0]}")"
output_dir="$(cd ../../tests/x86_64 && pwd)"
build_dir="$(mktemp -d)"
trap 'rm -rf -- "$build_dir"' EXIT

for variant in v4 v5_relative v5_absolute; do
    version=5
    absolute_dirs=0
    if [[ "$variant" == v4 ]]; then
        version=4
    elif [[ "$variant" == v5_absolute ]]; then
        absolute_dirs=1
    fi
    gcc -E -P -x assembler-with-cpp \
        -DDWARF_VERSION="$version" -DABSOLUTE_DIRS="$absolute_dirs" \
        line_filenames.S -o "$build_dir/line.s"
    as --64 --gdwarf-"$version" --debug-prefix-map="$PWD"=/build \
        -o "$build_dir/line.o" "$build_dir/line.s"
    ld -m elf_x86_64 --build-id=none -e _start -Ttext=0x401000 \
        -o "$output_dir/dwarf_line_filenames_$variant" "$build_dir/line.o"
done
