# DWARF line-table filename fixtures

Synthetic Linux x86-64 ELF executables for angr/cle#879 (angr/cle#548).
Each exits with status zero and has no shared-library dependencies.
The fixtures and their build sources are available under the MIT license;
see LICENSE.

Rebuild all three fixtures from this directory with `bash build.sh`.
Requires GCC for preprocessing and GNU binutils with DWARF 5 support.
The committed binaries were built with GNU binutils 2.42.
The assembler's compilation directory is mapped to `/build` so paths do not
depend on the checkout directory. The `.file` and `.loc` directives describe
synthetic source locations; no C files are needed to build these executables.

| Binary in `tests/x86_64` | DWARF version | File indices | Directory table |
| --- | --- | --- | --- |
| `dwarf_line_filenames_v4` | 4 | 1, 2 | `include` (index 0 means compilation directory) |
| `dwarf_line_filenames_v5_relative` | 5 | 0, 1 | `source`, `include` |
| `dwarf_line_filenames_v5_absolute` | 5 | 0, 1 | `/source`, `/include` |

The entry point is `0x401000`. Its first three instructions map to main.c:11,
header.h:23, and main.c:12 at offsets 0, 1, and 2. Returning to the main file
also exercises CLE's filename cache. `.cfi_startproc` emits `.eh_frame`, so
the binaries follow CLE's normal DWARF-loading path.

Inspect the actual tables with
`readelf --debug-dump=rawline --debug-dump=info ../../tests/x86_64/dwarf_line_filenames_v5_relative`.
