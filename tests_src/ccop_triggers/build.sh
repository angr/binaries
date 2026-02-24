#!/bin/bash
#
# Build all ccop trigger binaries for amd64 and i386.
# Usage: cd tests/analyses/decompiler/ccop_triggers && bash build.sh
#
# Produces multiple variants per source file using different optimization
# levels, tuning targets, and feature flags to maximize the variety of
# ccop patterns the compiler emits.
#
# Requires: gcc, gcc-multilib (for -m32 builds)
#
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

# Base flags: suppress conditional moves so branches stay as branches,
# keep debug info for decompiler friendliness
BASE="-fno-if-conversion -fno-if-conversion2 -fno-tree-loop-if-convert -g"

# ─── Standard sources: work fine with default tuning ──────────────
STD_SRCS=(
    ccop_sub
    ccop_add
    ccop_logic
    ccop_shl_shr
    ccop_umul_smul
    ccop_copy
    ccop_rflags_c
)

# ─── INC/DEC source: needs -mtune-ctrl=use_incdec to emit inc/dec ─
INCDEC_SRCS=(
    ccop_inc_dec
)

# ─── ADC/SBB source: needs wide types (__int128 on amd64, uint64_t
#     on i386). Standard flags are fine; the code itself triggers it.
ADCSBB_SRCS=(
    ccop_adc_sbb
)

# Standard optimization variants
STD_VARIANTS=(
    "O1::-O1"
    "O2::-O2"
    "Os::-Os"
)

# INC/DEC variants: force inc/dec emission via tuning control
# -mtune-ctrl=use_incdec: explicit GCC override (works with any -mtune)
# -mtune=haswell: Haswell naturally enables inc/dec
INCDEC_VARIANTS=(
    "O1_incdec::-O1 -mtune-ctrl=use_incdec"
    "O2_haswell::-O2 -mtune=haswell"
    "Os_incdec::-Os -mtune-ctrl=use_incdec"
)

mkdir -p bin/amd64 bin/i386

FAIL=0
COUNT=0

build_one() {
    local src="$1" name="$2" arch_flag="$3" extra_flags="$4" outdir="$5"
    if gcc $arch_flag $BASE $extra_flags -o "${outdir}/${name}" "${src}.c" 2>&1; then
        COUNT=$((COUNT + 1))
    else
        echo "  FAILED: $name ($outdir)"
        FAIL=1
    fi
}

# Build standard sources with standard variants
for src in "${STD_SRCS[@]}"; do
    for variant in "${STD_VARIANTS[@]}"; do
        IFS=':' read -r tag _ flags <<< "$variant"
        name="${src}_${tag}"
        echo "Building $name ..."
        build_one "$src" "$name" ""   "$flags" "bin/amd64"
        build_one "$src" "$name" "-m32" "$flags" "bin/i386"
    done
done

# Build INC/DEC sources with inc/dec-forcing variants
for src in "${INCDEC_SRCS[@]}"; do
    for variant in "${INCDEC_VARIANTS[@]}"; do
        IFS=':' read -r tag _ flags <<< "$variant"
        name="${src}_${tag}"
        echo "Building $name ..."
        build_one "$src" "$name" ""   "$flags" "bin/amd64"
        build_one "$src" "$name" "-m32" "$flags" "bin/i386"
    done
done

# Build ADC/SBB sources with standard variants
for src in "${ADCSBB_SRCS[@]}"; do
    for variant in "${STD_VARIANTS[@]}"; do
        IFS=':' read -r tag _ flags <<< "$variant"
        name="${src}_${tag}"
        echo "Building $name ..."
        build_one "$src" "$name" ""   "$flags" "bin/amd64"
        build_one "$src" "$name" "-m32" "$flags" "bin/i386"
    done
done

echo ""
if [ $FAIL -eq 0 ]; then
    echo "Done. Built $COUNT binaries in bin/amd64/ and bin/i386/"
else
    echo "Done with errors ($COUNT succeeded). Check output above."
    exit 1
fi
