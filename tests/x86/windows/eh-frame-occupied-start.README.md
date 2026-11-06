# eh-frame-occupied-start.exe

This inert PE fixture was authored for angr's static-analysis tests. Its entry
point returns immediately; never execute test fixtures.

The adjacent `eh-frame-occupied-start.S` is the complete source and is licensed
under BSD-2-Clause in `eh-frame-occupied-start.LICENSE`. It places a GNU
`.eh_frame` FDE at `0x40100a`. The entry point calls a wrapper at `0x401006`,
which tail-jumps to the FDE function. Without an authoritative start, CFG
recovery owns the target as part of the wrapper; this exercises splitting and
preserving an already occupied function boundary through reconstruction.

It was built once in the angr-agentic Nix environment with GCC 15.3.0 and GNU
Binutils 2.46. Tests load the committed PE and never assemble it at runtime:

```sh
export SOURCE_DATE_EPOCH=0
i686-w64-mingw32-gcc -nostdlib \
    -Wl,--entry,_start -Wl,--subsystem,console \
    -Wl,--image-base,0x400000 -Wl,--no-insert-timestamp \
    -Wl,--enable-long-section-names -Wl,--strip-all \
    -o eh-frame-occupied-start.exe eh-frame-occupied-start.S
```

The long-section-name option deliberately preserves the exact `.eh_frame`
name while the linker's `--strip-all` removes symbols. The PE timestamp and
debug-directory timestamp are zero, and repeating this command in separate
directories with the same output basename produces byte-identical files. The
final file has no symbol or debug tables.

- Source SHA-256: `1b2246525aca4cddbb510b4836116cf4aa78d8d4722ff4cb58caf7e029dbd99b`
- PE SHA-256: `1cd9ae0b00d1c4cde4cd95579cf9aae14104d9f99031aab77b0116e4e9aa7c8a`
