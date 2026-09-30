#!/bin/sh
# Build tests/aarch64/windows/pe_reloc_arm64.exe and tests/armel/windows/pe_reloc_armnt.exe,
# freestanding Windows images for the two ARM machine types.
#
# No C runtime is linked, so the images stay a few kilobytes and contain nothing but the
# compiled source: a .pdata exception directory in the ARM64 and ARMNT layouts, and a .reloc
# directory holding IMAGE_REL_BASED_DIR64 on ARM64 and IMAGE_REL_BASED_HIGHLOW together with
# IMAGE_REL_BASED_THUMB_MOV32 on ARMNT. /dynamicbase keeps the base relocations in the image.
#
# Requires: clang and lld-link (built with the AArch64 and ARM targets). Built with clang 21.1.8.
set -e

SRC=$(dirname "$0")/pe_reloc_arm.c
OUT=$(dirname "$0")/../../tests
CFLAGS="-O1 -fno-inline -funwind-tables -ffreestanding -fno-stack-protector"

clang --target=aarch64-pc-windows-msvc $CFLAGS -c "$SRC" -o pe_reloc_arm64.obj
lld-link /machine:arm64 /entry:mainCRTStartup /subsystem:console /nodefaultlib /dynamicbase \
    /out:"$OUT/aarch64/windows/pe_reloc_arm64.exe" pe_reloc_arm64.obj

clang --target=thumbv7-pc-windows-msvc $CFLAGS -c "$SRC" -o pe_reloc_armnt.obj
lld-link /machine:arm /entry:mainCRTStartup /subsystem:console /nodefaultlib /dynamicbase \
    /out:"$OUT/armel/windows/pe_reloc_armnt.exe" pe_reloc_armnt.obj

rm -f pe_reloc_arm64.obj pe_reloc_armnt.obj
