# ChibiOS `_crt0_entry` fixtures

These files are the 134-byte `_crt0_entry` function bodies from the public
DecBench ChibiOS Cortex-M binaries at dataset revision
[`e5eb576d66ee36793b800a4dd45e291e0add4472`](https://huggingface.co/datasets/noelo-lab/decbench-dataset/tree/e5eb576d66ee36793b800a4dd45e291e0add4472).
Each body starts at file offset `0x11e0` in its source ELF and is loaded at
address `0x080001e0`.

| Fixture | Source ELF | Source ELF SHA-256 |
| --- | --- | --- |
| `O0.bin` | [`binaries/O0/chibios/ch.elf`](https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/e5eb576d66ee36793b800a4dd45e291e0add4472/binaries/O0/chibios/ch.elf) | `275e02bf674b30c532bd52c3f725c08803b3a50f4af1eca53a3e3b4e687cae82` |
| `O2.bin` | [`binaries/O2/chibios/ch.elf`](https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/e5eb576d66ee36793b800a4dd45e291e0add4472/binaries/O2/chibios/ch.elf) | `93c66042d92118f1762e3b638f441889ee2e8117c4e24147ac1cb58979937ef4` |
| `O2-noinline.bin` | [`binaries/O2-noinline/chibios/ch.elf`](https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/e5eb576d66ee36793b800a4dd45e291e0add4472/binaries/O2-noinline/chibios/ch.elf) | `ab1ca2ae291d4aa14729ec5163c75046236397e6cf9d979f89809c3b0e2d5fa4` |

The binaries identify their ChibiOS/RT kernel as version 7.0.6. The matching
upstream release is ChibiOS 21.11.5 at commit
[`f4bbadf964fc746aef8bbcf34135c7d8fabb8eae`](https://github.com/ChibiOS/ChibiOS/tree/f4bbadf964fc746aef8bbcf34135c7d8fabb8eae).
The function source is
[`os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S`](https://github.com/ChibiOS/ChibiOS/blob/f4bbadf964fc746aef8bbcf34135c7d8fabb8eae/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S).

ChibiOS is licensed under the GNU General Public License version 3. The
adjacent `LICENSE` is the verbatim license file from that pinned upstream
revision.
