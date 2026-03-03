#!/bin/bash
#
# Build all ccop trigger binaries for amd64, i386, armhf, and aarch64.
# Usage: cd tests_src/ccop_triggers && bash build.sh
#
# Produces multiple variants per source file using different optimization
# levels, tuning targets, and feature flags to maximize the variety of
# ccop patterns the compiler emits.
#
# Requires:
#   gcc, gcc-multilib (for -m32 builds)
#   arm-linux-gnueabihf-gcc (for armhf builds)
#   aarch64-linux-gnu-gcc (for aarch64 builds)
#
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# Base flags: suppress conditional moves so branches stay as branches,
# keep debug info for decompiler friendliness
BASE="-fno-if-conversion -fno-if-conversion2 -fno-tree-loop-if-convert -g"

# ─── x86/amd64: Standard sources ─────────────────────────────────
STD_SRCS=(
    ccop_sub
    ccop_add
    ccop_logic
    ccop_shl_shr
    ccop_umul_smul
    ccop_copy
    ccop_rflags_c
)

# ─── x86/amd64: INC/DEC source ───────────────────────────────────
INCDEC_SRCS=(
    ccop_inc_dec
)

# ─── x86/amd64: ADC/SBB source ───────────────────────────────────
ADCSBB_SRCS=(
    ccop_adc_sbb
)

# ─── ARM: Sources that work on both armhf and aarch64 ────────────
ARM_SRCS=(
    ccop_arm_sub
    ccop_arm_add
    ccop_arm_logic
    ccop_arm_adc_sbb
)

# Standard optimization variants
STD_VARIANTS=(
    "O1::-O1"
    "O2::-O2"
    "Os::-Os"
)

# INC/DEC variants: force inc/dec emission via tuning control
INCDEC_VARIANTS=(
    "O1_incdec::-O1 -mtune-ctrl=use_incdec"
    "O2_haswell::-O2 -mtune=haswell"
    "Os_incdec::-Os -mtune-ctrl=use_incdec"
)

mkdir -p bin/amd64 bin/i386 bin/armhf bin/aarch64

FAIL=0
COUNT=0

build_one() {
    local compiler="$1" src="$2" name="$3" extra_flags="$4" outdir="$5"
    if $compiler $BASE $extra_flags -o "${outdir}/${name}" "${src}.c" 2>&1; then
        COUNT=$((COUNT + 1))
    else
        echo "  FAILED: $name ($outdir)"
        FAIL=1
    fi
}

# ═══════════════════════════════════════════════════════════════════
# x86 / amd64 builds
# ═══════════════════════════════════════════════════════════════════

# Build standard sources with standard variants
for src in "${STD_SRCS[@]}"; do
    for variant in "${STD_VARIANTS[@]}"; do
        IFS=':' read -r tag _ flags <<< "$variant"
        name="${src}_${tag}"
        echo "Building $name ..."
        build_one "gcc"       "$src" "$name" "$flags"      "bin/amd64"
        build_one "gcc -m32"  "$src" "$name" "$flags"      "bin/i386"
    done
done

# Build INC/DEC sources with inc/dec-forcing variants
for src in "${INCDEC_SRCS[@]}"; do
    for variant in "${INCDEC_VARIANTS[@]}"; do
        IFS=':' read -r tag _ flags <<< "$variant"
        name="${src}_${tag}"
        echo "Building $name ..."
        build_one "gcc"       "$src" "$name" "$flags"      "bin/amd64"
        build_one "gcc -m32"  "$src" "$name" "$flags"      "bin/i386"
    done
done

# Build ADC/SBB sources with standard variants
for src in "${ADCSBB_SRCS[@]}"; do
    for variant in "${STD_VARIANTS[@]}"; do
        IFS=':' read -r tag _ flags <<< "$variant"
        name="${src}_${tag}"
        echo "Building $name ..."
        build_one "gcc"       "$src" "$name" "$flags"      "bin/amd64"
        build_one "gcc -m32"  "$src" "$name" "$flags"      "bin/i386"
    done
done

# ═══════════════════════════════════════════════════════════════════
# ARM builds (armhf + aarch64)
# ═══════════════════════════════════════════════════════════════════

for src in "${ARM_SRCS[@]}"; do
    for variant in "${STD_VARIANTS[@]}"; do
        IFS=':' read -r tag _ flags <<< "$variant"
        name="${src}_${tag}"
        echo "Building $name ..."
        build_one "arm-linux-gnueabihf-gcc"  "$src" "$name" "$flags"  "bin/armhf"
        build_one "aarch64-linux-gnu-gcc"    "$src" "$name" "$flags"  "bin/aarch64"
    done
done

echo ""
if [ $FAIL -eq 0 ]; then
    echo "Done. Built $COUNT binaries."
else
    echo "Done with errors ($COUNT succeeded). Check output above."
    exit 1
fi
