# Thumb guarded-load fixture

`loadg_ite_thumb.elf` is a real toolchain-produced ELF32 ARM executable for
testing VEX `LoadG` conversion. Its only function has this calling convention:

```c
uint32_t loadg_ite_mask(const volatile uint32_t *base,
                        uint32_t selector, uint32_t mask);
```

The function uses an explicit Thumb `ITE EQ` block. An even `selector` loads
`base[0]`; an odd selector loads `base[1]`. It returns the selected word ANDed
with `mask`. The two complementary conditional loads lift as
two little-endian VEX `LoadG` statements, each using `ILGop_Ident32`.

The source at
[`tests_src/decompiler/loadg_ite_thumb.S`](../../../tests_src/decompiler/loadg_ite_thumb.S)
was independently authored for this fixture and carries the SPDX identifier
`MIT`. No libraries or other object files were linked; the executable's
`.text` is derived solely from that source. The source and resulting executable
are distributed under this repository's MIT license. Tests load the committed
ELF; they do not run this build recipe.

## Reproduction

The fixture was built in the workspace's supported Nix environment with Clang
21.1.8's integrated assembler and LLD 21.1.8. From a fresh directory containing
the source, the exact commands were:

```sh
clang --target=arm-none-eabi -mcpu=cortex-m3 -mthumb -mfloat-abi=soft \
  -c loadg_ite_thumb.S -o loadg_ite_thumb.o
ld.lld -flavor gnu -m armelf --image-base=0x10000 \
  -z max-page-size=0x1000 --entry=loadg_ite_mask --build-id=none \
  --gc-sections -o loadg_ite_thumb.elf loadg_ite_thumb.o
```

Two fresh build directories produced byte-identical objects and executables.
The committed ELF is 660 bytes with SHA-256
`1c75e244c948d983c81f93e2c268c286316882df0168856c8c26195c6a8a0d8a`.
It is EABI5 soft-float ARM (`e_flags = 0x5000200`) with entry point `0x110b5`.
The symbol table retains `loadg_ite_mask` at `0x110b5` with size 16 bytes. The
file has no dynamic section, relocations, debug sections, source path, build ID,
or timestamp.
