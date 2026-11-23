# Thumb IT lookback-width fixture

This 64-byte raw `.text` image is built from the angr-authored MIT-licensed
source at `tests_src/thumb_it_lookback_widths.S`. It exercises Thumb IT blocks
containing both 16-bit and 32-bit instructions, including a 32-bit `LDREX`
whose second halfword is the false IT candidate `0xbf08`.

## Layout

The linked `.text` VMA begins at `0x110b4`; offsets below are relative to the
raw binary.

| Symbol or encoding | Raw offset | Linked address |
| --- | ---: | ---: |
| `mixed_width_expired` | `0x00` | `0x110b4` |
| `mixed_width_after` | `0x12` | `0x110c6` |
| `older_candidate_active` | `0x14` | `0x110c8` |
| `LDREX` bytes `50 e8 08 bf` | `0x20` | `0x110d4` |
| false `0xbf08` halfword | `0x22` | `0x110d6` |
| `older_candidate_current` | `0x26` | `0x110da` |
| `window_edge_expired` | `0x2c` | `0x110e0` |
| `window_edge_after` | `0x3e` | `0x110f2` |

## Toolchain and build

The fixture was built with Clang 21.1.8, LLD 21.1.8, and LLVM objcopy 21.1.8
from `nixpkgs-unstable`:

```sh
clang --target=arm-none-eabi -mcpu=cortex-m3 -mthumb -mfloat-abi=soft \
  -c thumb_it_lookback_widths.S -o thumb_it_lookback_widths.o
ld.lld -flavor gnu -m armelf --image-base=0x10000 -z max-page-size=0x1000 \
  --entry=mixed_width_expired --build-id=none --gc-sections \
  -o thumb_it_lookback_widths.elf thumb_it_lookback_widths.o
llvm-objcopy -O binary --only-section=.text \
  thumb_it_lookback_widths.elf thumb_it_lookback_widths.bin
```

Two builds in separate fresh directories produced identical files:

- source: `8da57e517dc8c7edf832e5dada0d4dd280c95184c5d7f375a418270503eee791`
- object: `097a0865be6da70e55e5280561c1dda192defcfa1e8ba7a3b412aee946ab4fd3`
- ELF: `6d5b81b5d5d49f3da14cb994ef8cac6190a23d86362968c4236b6f46982372ea`
- raw binary: `a49c1a151765f87446da09b98d6b11991f054a0a4ccb1d4ed2ea92694ad1f628`
