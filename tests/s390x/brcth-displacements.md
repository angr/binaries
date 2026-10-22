# BRCTH displacement fixture

`brcth-displacements.bin` is the `.text` section assembled from
`tests_src/s390x_brcth.s`. It contains four six-byte `brcth %r1` instructions,
with halfword displacements `+0x10000`, `+3`, `-1`, and `-0x10000`, in that order.
Each instruction is lifted separately by pyvex's BRCTH regression.

Rebuild from the repository root with Clang 21.1.8 and GNU objcopy 2.46:

```sh
clang --target=s390x-linux-gnu -march=z196 -c tests_src/s390x_brcth.s -o brcth.o
objcopy -I elf64-big -O binary -j .text brcth.o tests/s390x/brcth-displacements.bin
```

No relocations are needed: the source expresses each target relative to its
instruction. The binary is 24 bytes, with hex contents:

```text
cc1600010000cc1600000003cc16ffffffffcc16ffff0000
```
