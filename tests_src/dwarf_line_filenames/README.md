# DWARF line-table filename fixtures

Linux x86-64 ELF executables for angr/cle#879 (angr/cle#548), compiled from
the same `main.c` and `include/header.h`. The compiler generates all file
indices and line numbers. `main` calls `increment` in the header and returns
zero when it produces the expected result.

| Binary in `tests/x86_64` | Compilation |
| --- | --- |
| `dwarf_line_filenames_v4` | DWARF 4, relative source path |
| `dwarf_line_filenames_v5_relative` | DWARF 5, relative source path |
| `dwarf_line_filenames_v5_absolute` | DWARF 5, absolute source path |

The committed binaries were built with GCC 13.3.0 and GNU binutils 2.42 on
Ubuntu 24.04. `build.sh` records the compilation commands. Its output may
differ with another toolchain or checkout path.

The fixtures and their sources are available under the MIT license; see LICENSE.
