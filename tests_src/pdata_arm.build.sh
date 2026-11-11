#!/bin/sh
# Build pdata_arm64.exe and pdata_armnt.exe: ARM64 and ARMNT (Thumb-2) PEs whose exception
# directories (.pdata) hold both packed and .xdata unwind entries.
#
# Requires llvm-mingw (https://github.com/mstorsjo/llvm-mingw, built with 20261104); set
# LLVM_MINGW to its bin directory if the compilers are not on PATH.
# Inspect the result with: llvm-readobj --unwind <exe>
set -e

BIN=${LLVM_MINGW:+${LLVM_MINGW}/}
HERE=$(dirname "$0")
OUT=${HERE}/../tests

for target in aarch64:aarch64/windows/pdata_arm64.exe armv7:armel/windows/pdata_armnt.exe; do
    triple=${target%%:*}-w64-mingw32
    exe=${OUT}/${target#*:}
    # --no-insert-timestamp keeps rebuilds byte-identical
    "${BIN}${triple}-clang" -O2 -fno-omit-frame-pointer -o "${exe}" "${HERE}/pdata_arm.c" \
        -Wl,--no-insert-timestamp
done
