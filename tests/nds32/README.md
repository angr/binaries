# NDS32 test binaries

`crt0_nds32le.o` is `nds32le-elf/lib/crt0.o` from Andes Technology's newlib
toolchain release `ast-v3_2_4-release-linux`, taken from
`nds32le-elf-newlib-v3.txz` at
<https://github.com/andestech/nds-toolchain/releases/tag/ast-v3_2_4-release-linux>
(tarball sha256
`91a47312540de747f9761384a0b127be9519956b8e32f0a762015ba5639954a5`). It is a
little-endian NDS32 relocatable object: `e_machine` 167, `EI_DATA`
`ELFDATA2LSB`, `e_flags` `0x30000042`.

newlib is distributed under BSD-style licenses; see the `LICENSE` file of the
toolchain release for the terms covering this file.
