# 2048 runtime-debugger fixtures

These x86-64 Linux executables are built from the unmodified MIT-licensed
[2048.c](https://github.com/mevdschee/2048.c/blob/90343eb209e84fa9687d596f5d5eecfdf7d4df51/2048.c)
at commit `90343eb209e84fa9687d596f5d5eecfdf7d4df51`. The source and upstream
license are included here.

| File | Purpose |
| --- | --- |
| `2048.pie` | PIE with DWARF, for randomized runtime addresses, stack locals, pointer arguments, and stepping |
| `2048.nonpie` | The same source with a fixed executable load address |
| `2048.stripped` | Stripped copy of the PIE, for resolving analysis addresses without symbols |

The game needs Linux x86-64 and glibc 2.34 or newer. The ELF interpreter is
`/lib64/ld-linux-x86-64.so.2`; no Nix interpreter or library search path is
required. No executable fixture is built during a test.

Run any executable with `test` for the game's 13 deterministic algorithm tests,
without a display or terminal input. Run without arguments for the interactive
game, using arrow keys or WASD. The test mode calls
`slideArray(uint8_t array[4], uint32_t *score)`, which is useful for inspecting
and modifying a pointer argument and observing score changes. `testSucceed` and
`main` contain stack-local score values and arrays.

## Build

`build.nix` pins Nixpkgs at `6b5e5b7a6631f065bf6908986990b37d845f847f`, which
provides GCC 15.3.0, GNU binutils, and patchelf. From this directory:

```sh
nix-build build.nix --out-link /path/to/persistent/scratch/runtime-2048
```

The recipe compiles with `-std=c99 -O0 -g -fno-omit-frame-pointer`, plus
`-fPIE -pie` or `-fno-PIE -no-pie`. For reproducible builds, it maps the build
directory to `.` in DWARF, uses a fixed compiler seed, and omits the
build-output RPATH from linking. It then sets the system ELF interpreter
and removes the compiler's Nix RPATH.
`2048.stripped` is produced with `strip --strip-all` from `2048.pie`.
A forced `nix-build --check` rebuild reproduced all three outputs byte for byte.

SHA-256:

```text
0b83657775abad063068a565fe8f90227797b8f2a94e1ed15dfdde8cacc4f579  2048.pie
d0e5b29c42328c9aef791f2fa3ea9b8ee23e38bfce05007ab6aeee87ff842a71  2048.nonpie
34e02dea450bb47243dabeca36ca227fc7b66a0b7a2df0d6c0ff2c96855fd361  2048.stripped
90f794c675a696aa10980dd120d1f0503ee4ed33a61fef92c7df8f443f5b18a8  2048.c
```
