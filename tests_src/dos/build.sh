#!/usr/bin/env bash
# Build the three DOS fixtures. Reproducible: every tool comes from a pinned
# nixpkgs revision, nothing is taken from PATH.
#
# nixpkgs rev 42f17a57f4f6e33b3de3dca0a2a5ea5233169d02 (nixpkgs-unstable,
# last modified 2026-09-07), which is what "nix flake metadata nixpkgs"
# resolved to on this machine. Note this is NOT the workspace flake's own pin
# (flake.lock holds 6b5e5b7a6631f065bf6908986990b37d845f847f); the fixtures do
# not go through the workspace env, they are built straight from nixpkgs.
#
# Tools:
#   nasm 3.02              - assembles the .COM directly (nasm -f bin)
#   open-watcom-bin 1.9    - wasm assembles to OMF, wlink links the MZ .EXE
#   fasm 1.73.35           - flat assembler; its "format MZ" writer emits the
#                            tiny-model .EXE whose header CS is negative
#
# open-watcom is marked unfree in nixpkgs (the Sybase Open Watcom Public
# License), hence NIXPKGS_ALLOW_UNFREE=1 and --impure. Only the toolchain is
# under that licence; the fixture is our own assembly and links no Watcom
# runtime library.

set -euo pipefail

NIXPKGS="github:NixOS/nixpkgs/42f17a57f4f6e33b3de3dca0a2a5ea5233169d02"
BASE_DIR=$(cd "$(dirname "$0")" && pwd)
export OUTPUT_DIR=$BASE_DIR/../../tests/i386/dos
export SCRATCH
SCRATCH=$(mktemp -d)
trap 'rm -rf "$SCRATCH"' EXIT
cd "$BASE_DIR"

# ---- hello.com : flat .COM, org 0x100, no header ----
nix shell "${NIXPKGS}#nasm" --command \
    nasm -f bin -o "$OUTPUT_DIR/hello.com" dos_com.asm

# ---- hello.exe : real-mode MZ .EXE ----
#
# wasm flags:
#   -zq   quiet
#   -0    8086 instruction set only
#   -zld  do not emit the OMF records that let wlink collapse far calls
#
# wlink options:
#   system dos        16-bit real-mode MZ executable
#   option nofarcalls keep "call far ptr" as a real 9A far call, so its segment
#                     word stays in the relocation table. Without this wlink
#                     rewrites it to "push cs / call near" and e_crlc drops to 4.
#   option stack=512  512-byte stack instead of wlink's 4 KB default, which
#                     otherwise pads the load module out to 4354 bytes
#   option map=       write a link map; nothing reads it and this script throws
#                     it away with the scratch directory, but it is what the
#                     segment layout was read from when the fixture was designed
#   name              the output file
#
# wasm and wlink both write their outputs next to the input, so this builds in a
# scratch directory and moves the executable to $OUTPUT_DIR.
NIXPKGS_ALLOW_UNFREE=1 nix shell --impure "${NIXPKGS}#open-watcom-bin" --command sh -c '
    set -e
    wasm -zq -0 -zld -fo="$SCRATCH/dos_mz.obj" dos_mz.asm
    wlink system dos \
          option nofarcalls \
          option stack=512 \
          option map="$SCRATCH/dos_mz.map" \
          name "$SCRATCH/hello.exe" \
          file "$SCRATCH/dos_mz.obj"
    mv "$SCRATCH/hello.exe" "$OUTPUT_DIR/hello.exe"
'

# ---- hello_tiny.exe : tiny-model MZ .EXE, negative header CS ----
#
# wlink cannot produce this: its MS-DOS format options are only FULLHeader,
# PACKCode and PACKData, and "option start=" puts the entry on whatever
# paragraph the symbol landed on, never below the load module. fasm can,
# because the source states the entry and the stack as segment:offset pairs and
# fasm's own MZ writer turns "code_seg-10h" into e_cs = 0FFF0h. No header byte
# is written by hand. fasm is free software, so this needs no --impure.
nix shell "${NIXPKGS}#fasm" --command \
    fasm dos_mz_tiny.asm "$OUTPUT_DIR/hello_tiny.exe"

cd "$OUTPUT_DIR"
ls -l hello.com hello.exe hello_tiny.exe
sha256sum hello.com hello.exe hello_tiny.exe

# Expected, in that order:
#   5bab2c65c03eff9853f3ca74ed52881e37a043873fa15e769fc1dac0ade358f5  hello.com
#   157b8cb060ae2ab393fa0b2d02edc321fca003a80bdb6023a529ffc6570af22b  hello.exe
#   758b1946baba075466f137b9725d466124b8bcbadc28c94bd1f6dde1a98fef0b  hello_tiny.exe
#
# The script overwrites the committed fixtures in place, so "git status" is the
# check that it reproduced them: a clean tree means the bytes are identical.
