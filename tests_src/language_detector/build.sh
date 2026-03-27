#!/usr/bin/env bash
# Build script for language detector test binaries.
#
# Produces both static and dynamic variants for each language/compiler.
# Output directory: tests/x86_64/ (relative to the binaries repo root).
#
# Requirements:
#   - gcc
#   - rustc
#   - go  (set GO_BIN to override path, e.g. GO_BIN=/home/node/go/bin/go)
#
# Usage:
#   cd <binaries-repo-root>/tests_src/language_detector
#   ./build.sh

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
OUT_DIR="$(cd "$SCRIPT_DIR/../../tests/x86_64" && pwd)"

GO_BIN="${GO_BIN:-go}"

echo "Source dir : $SCRIPT_DIR"
echo "Output dir : $OUT_DIR"
echo ""

# ---- helpers ----

ok()   { echo "  OK: $1"; }
fail() { echo "  FAIL: $1 ($2)"; }

# ---- C / gcc (static) ----

echo "=== C / gcc (static) ==="

gcc -o "$OUT_DIR/langdetect_gcc" "$SCRIPT_DIR/langdetect_gcc.c" -static \
    && ok langdetect_gcc || fail langdetect_gcc "gcc not found?"

gcc -g -o "$OUT_DIR/langdetect_gcc_dwarf" "$SCRIPT_DIR/langdetect_gcc.c" -static \
    && ok langdetect_gcc_dwarf || fail langdetect_gcc_dwarf "gcc -g"

gcc -o "$OUT_DIR/langdetect_gcc_stripped" "$SCRIPT_DIR/langdetect_gcc_stripped.c" -static \
    && strip --strip-all --remove-section=.comment "$OUT_DIR/langdetect_gcc_stripped" \
    && ok langdetect_gcc_stripped || fail langdetect_gcc_stripped "strip"

# ---- C / gcc (dynamic) ----

echo "=== C / gcc (dynamic) ==="

gcc -o "$OUT_DIR/langdetect_gcc_dyn" "$SCRIPT_DIR/langdetect_gcc.c" \
    && ok langdetect_gcc_dyn || fail langdetect_gcc_dyn ""

gcc -g -o "$OUT_DIR/langdetect_gcc_dwarf_dyn" "$SCRIPT_DIR/langdetect_gcc.c" \
    && ok langdetect_gcc_dwarf_dyn || fail langdetect_gcc_dwarf_dyn ""

gcc -o "$OUT_DIR/langdetect_gcc_stripped_dyn" "$SCRIPT_DIR/langdetect_gcc_stripped.c" \
    && strip --strip-all --remove-section=.comment "$OUT_DIR/langdetect_gcc_stripped_dyn" \
    && ok langdetect_gcc_stripped_dyn || fail langdetect_gcc_stripped_dyn ""

# ---- C / fake-clang (static + dynamic) ----
# Compiled with gcc, then .comment section is replaced with a clang identifier.

echo "=== C / fake-clang ==="

CLANG_COMMENT="$(mktemp)"
printf 'clang version 15.0.7 (https://github.com/llvm/llvm-project)\0' > "$CLANG_COMMENT"

for variant in "" "_dyn"; do
    static_flag=""
    [ -z "$variant" ] && static_flag="-static"

    name="langdetect_clang${variant}"
    gcc $static_flag -o "$OUT_DIR/$name" "$SCRIPT_DIR/langdetect_clang.c" \
        && objcopy --remove-section=.comment "$OUT_DIR/$name" \
        && objcopy --add-section ".comment=$CLANG_COMMENT" "$OUT_DIR/$name" \
        && ok "$name" || fail "$name" ""
done

rm -f "$CLANG_COMMENT"

# ---- Rust (static + dynamic) ----

echo "=== Rust ==="

rustc -o "$OUT_DIR/langdetect_rust" "$SCRIPT_DIR/langdetect_rust.rs" \
    -C target-feature=+crt-static \
    && ok langdetect_rust || fail langdetect_rust "rustc not found?"

rustc -o "$OUT_DIR/langdetect_rust_dyn" "$SCRIPT_DIR/langdetect_rust.rs" \
    && ok langdetect_rust_dyn || fail langdetect_rust_dyn ""

# ---- Go (static + dynamic) ----

echo "=== Go ==="

"$GO_BIN" build -o "$OUT_DIR/langdetect_go" "$SCRIPT_DIR/langdetect_go.go" \
    && ok langdetect_go || fail langdetect_go "go not found? set GO_BIN"

CGO_ENABLED=1 "$GO_BIN" build \
    -o "$OUT_DIR/langdetect_go_dyn" \
    -ldflags="-linkmode=external" \
    "$SCRIPT_DIR/langdetect_go.go" \
    && ok langdetect_go_dyn || fail langdetect_go_dyn ""

# ---- Swift (fake, static + dynamic) ----
# Compiled with gcc + assembly stubs providing Swift-like symbols and sections.

echo "=== Swift (fake) ==="

TMPDIR="$(mktemp -d)"

gcc -c -o "$TMPDIR/swift_symbols.o" "$SCRIPT_DIR/langdetect_swift_symbols.s"
gcc -c -o "$TMPDIR/swift_main.o" "$SCRIPT_DIR/langdetect_swift.c" -static

# static
gcc -o "$OUT_DIR/langdetect_swift" "$TMPDIR/swift_main.o" "$TMPDIR/swift_symbols.o" \
    -static -no-pie \
    && objcopy --remove-section=.comment "$OUT_DIR/langdetect_swift" \
    && ok langdetect_swift || fail langdetect_swift ""

# dynamic
gcc -c -o "$TMPDIR/swift_main_dyn.o" "$SCRIPT_DIR/langdetect_swift.c"
gcc -o "$OUT_DIR/langdetect_swift_dyn" "$TMPDIR/swift_main_dyn.o" "$TMPDIR/swift_symbols.o" \
    -no-pie \
    && objcopy --remove-section=.comment "$OUT_DIR/langdetect_swift_dyn" \
    && ok langdetect_swift_dyn || fail langdetect_swift_dyn ""

rm -rf "$TMPDIR"

echo ""
echo "Done. Binaries in $OUT_DIR/langdetect_*:"
ls -lh "$OUT_DIR"/langdetect_* 2>/dev/null
