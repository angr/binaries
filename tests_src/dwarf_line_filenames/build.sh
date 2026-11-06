#!/bin/sh
set -eu

cd -- "$(dirname -- "$0")"
output_dir=../../tests/x86_64
gcc -O0 -g -gdwarf-4 -no-pie main.c -o "$output_dir/dwarf_line_filenames_v4"
gcc -O0 -g -gdwarf-5 -no-pie main.c -o "$output_dir/dwarf_line_filenames_v5_relative"
gcc -O0 -g -gdwarf-5 -no-pie "$PWD/main.c" -o "$output_dir/dwarf_line_filenames_v5_absolute"
