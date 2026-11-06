#!/usr/bin/env bash
# Build the two NE fixtures. Reproducible: every tool comes from a pinned
# nixpkgs revision, nothing is taken from PATH.
#
# The toolchain is the one tests_src/dos/build.sh uses: open-watcom-bin 1.9
# from nixpkgs rev 42f17a57f4f6e33b3de3dca0a2a5ea5233169d02. wasm assembles
# each source to OMF and wlink writes the NE executable, so every header and
# table is the linker's own output.
#
# open-watcom is marked unfree in nixpkgs (the Sybase Open Watcom Public
# License), hence NIXPKGS_ALLOW_UNFREE=1 and --impure. Only the toolchain is
# under that licence; the fixtures are our own assembly and link no Watcom
# runtime library.
#
# wasm flags:
#   -zq   quiet
#   -2    80286 instruction set, for push imm16
#
# wlink reads its directives from win16.lnk and os2.lnk:
#   system windows    Windows 3.x NE executable, target OS byte 2
#   system os2        OS/2 1.x NE executable, target OS byte 1
#   option packcode=0 keep MAIN_TEXT and MSG_TEXT as two segments, so the far
#                     call between them is an NE fixup; by default wlink
#                     packs them into one
#   segment type code moveable discardable
#                     the usual Windows code segment attributes; because
#                     ShowCaption is in a movable segment, wlink reaches it
#                     through its entry-table ordinal, which is the fixup form
#                     a movable target needs
#   option description
#                     the module description, the first entry of the
#                     nonresident-name table; without it wlink writes the
#                     output file name there
#   option map=       write a link map; nothing reads it and this script
#                     throws it away with the scratch directory
#   import NAME MODULE.ORDINAL | MODULE.NAME
#                     declare each imported procedure directly, so no Watcom
#                     import library is linked; WAITEVENT is imported by name
#                     and the rest by ordinal
#   export ShowCaption
#                     put ShowCaption in the entry table and the resident
#                     names
#
# Both tools run in a scratch directory, which holds the objects, the link
# maps and the executables until the executables are moved to $OUTPUT_DIR.

set -euo pipefail

NIXPKGS="github:NixOS/nixpkgs/42f17a57f4f6e33b3de3dca0a2a5ea5233169d02"
BASE_DIR=$(cd "$(dirname "$0")" && pwd)
export OUTPUT_DIR=$BASE_DIR/../../tests/i386/ne
export SCRATCH
SCRATCH=$(mktemp -d)
trap 'rm -rf "$SCRATCH"' EXIT

export SOURCE_DIR=$BASE_DIR
cd "$SCRATCH"
NIXPKGS_ALLOW_UNFREE=1 nix shell --impure "${NIXPKGS}#open-watcom-bin" --command sh -c '
    set -e
    for name in win16 os2; do
        wasm -zq -2 -fo=$name.obj "$SOURCE_DIR/$name.asm"
        wlink @"$SOURCE_DIR/$name.lnk"
        mv $name.exe "$OUTPUT_DIR/$name.exe"
    done
'

cd "$OUTPUT_DIR"
ls -l win16.exe os2.exe
sha256sum win16.exe os2.exe

# Expected, in that order:
#   421884f20ae192f4af3d286351fd623055e1d7ab5f72b6a2854406835362cb27  win16.exe
#   35814bd80abdf8dea5167e0b9a9785165caab0105c0d5b1f4d083090d2b466d5  os2.exe
#
# The script overwrites the committed fixtures in place, so "git status" is the
# check that it reproduced them: a clean tree means the bytes are identical.
