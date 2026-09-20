#!/bin/bash
#
# Build recompile-dataset binaries for multiple architectures, compilers,
# and optimisation levels.
#
# Usage: cd tests_src/recompile_dataset && bash build.sh
#
# Requires (install missing ones with apt):
#   gcc, gcc-multilib            (x86_64 and i386)
#   aarch64-linux-gnu-gcc        (aarch64)
#   arm-linux-gnueabihf-gcc      (armhf)
#   clang                        (x86_64 and i386, and cross targets)
#   x86_64-w64-mingw32-gcc       (optional, for mingw PE builds)
#   i686-w64-mingw32-gcc         (optional, for mingw PE i386 builds)
#
set -e

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
cd "$SCRIPT_DIR"

SRCS=(
    t1_control_flow
    t2_types
    t3_memory
    t4_calling
    t5_patterns
)

# Output directories (relative to the binaries repo root)
REPO_ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
OUT_X64="$REPO_ROOT/tests/x86_64/recompile_dataset"
OUT_I386="$REPO_ROOT/tests/i386/recompile_dataset"
OUT_AARCH64="$REPO_ROOT/tests/aarch64/recompile_dataset"
OUT_ARMHF="$REPO_ROOT/tests/armhf/recompile_dataset"

mkdir -p "$OUT_X64" "$OUT_I386" "$OUT_AARCH64" "$OUT_ARMHF"

# Base flags: keep debug info.
# GCC supports -fno-if-conversion to keep branches; clang does not.
BASE_GCC="-g -fno-if-conversion -fno-if-conversion2"
BASE_CLANG="-g"

OPT_LEVELS=(-O0 -O1 -O2 -O3 -Os)

FAIL=0
COUNT=0
SKIP=0

build_one() {
    local compiler="$1"
    local src="$2"
    local name="$3"
    local flags="$4"
    local outdir="$5"
    local base="$6"

    if $compiler $base $flags \
        -o "${outdir}/${name}" \
        "${src}.c" dummy_main.c 2>/dev/null; then
        COUNT=$((COUNT + 1))
    else
        echo "  FAILED: $name ($outdir)"
        SKIP=$((SKIP + 1))
    fi
}

# Check which compilers are available
has_cmd() { command -v "$1" >/dev/null 2>&1; }

echo "=== Building recompile-dataset binaries ==="
echo ""

# ── GCC x86_64 + i386 ────────────────────────────────────────────
if has_cmd gcc; then
    for src in "${SRCS[@]}"; do
        for opt in "${OPT_LEVELS[@]}"; do
            tag="${opt#-}"  # O0, O1, ...
            echo "gcc x86_64: ${src}_gcc_${tag}"
            build_one "gcc" "$src" "${src}_gcc_${tag}" "$opt" "$OUT_X64" "$BASE_GCC"
            # i386 requires gcc-multilib; skip gracefully if missing
            echo "gcc i386:   ${src}_gcc_${tag}"
            build_one "gcc -m32" "$src" "${src}_gcc_${tag}" "$opt" "$OUT_I386" "$BASE_GCC"
        done
    done
else
    echo "SKIP: gcc not found"
fi

# ── Clang x86_64 + i386 ──────────────────────────────────────────
if has_cmd clang; then
    for src in "${SRCS[@]}"; do
        for opt in "${OPT_LEVELS[@]}"; do
            tag="${opt#-}"
            echo "clang x86_64: ${src}_clang_${tag}"
            build_one "clang" "$src" "${src}_clang_${tag}" "$opt" "$OUT_X64" "$BASE_CLANG"
            echo "clang i386:   ${src}_clang_${tag}"
            build_one "clang -m32" "$src" "${src}_clang_${tag}" "$opt" "$OUT_I386" "$BASE_CLANG"
        done
    done
else
    echo "SKIP: clang not found"
fi

# ── GCC aarch64 ──────────────────────────────────────────────────
if has_cmd aarch64-linux-gnu-gcc; then
    for src in "${SRCS[@]}"; do
        for opt in "${OPT_LEVELS[@]}"; do
            tag="${opt#-}"
            echo "aarch64-gcc: ${src}_gcc_${tag}"
            build_one "aarch64-linux-gnu-gcc" "$src" "${src}_gcc_${tag}" "$opt" "$OUT_AARCH64" "$BASE_GCC"
        done
    done
else
    echo "SKIP: aarch64-linux-gnu-gcc not found"
fi

# ── GCC armhf ────────────────────────────────────────────────────
if has_cmd arm-linux-gnueabihf-gcc; then
    for src in "${SRCS[@]}"; do
        for opt in "${OPT_LEVELS[@]}"; do
            tag="${opt#-}"
            echo "armhf-gcc: ${src}_gcc_${tag}"
            build_one "arm-linux-gnueabihf-gcc" "$src" "${src}_gcc_${tag}" "$opt" "$OUT_ARMHF" "$BASE_GCC"
        done
    done
else
    echo "SKIP: arm-linux-gnueabihf-gcc not found"
fi

# ── Clang aarch64 ────────────────────────────────────────────────
if has_cmd clang; then
    if has_cmd aarch64-linux-gnu-gcc; then
        for src in "${SRCS[@]}"; do
            for opt in "${OPT_LEVELS[@]}"; do
                tag="${opt#-}"
                echo "clang aarch64: ${src}_clang_${tag}"
                build_one "clang --target=aarch64-linux-gnu --sysroot=/usr/aarch64-linux-gnu" \
                    "$src" "${src}_clang_${tag}" "$opt" "$OUT_AARCH64" "$BASE_CLANG"
            done
        done
    fi
fi

echo ""
echo "Done. Built $COUNT binaries ($SKIP skipped)."
if [ $COUNT -eq 0 ]; then
    echo "ERROR: no binaries were built at all."
    exit 1
fi
