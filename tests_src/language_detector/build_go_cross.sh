#!/usr/bin/env bash
# Cross-compiled Go variants of the language detector, for CFG tests that need a Go binary of a
# specific architecture. Pure Go with CGO disabled cross-compiles with GOOS/GOARCH alone.
#
# Each target is built with several Go releases, because the goroutine stack-growth preamble the CFG
# recognizes has changed shape over time (windows/386 reaches g through fs:[0x14] up to go1.20 and
# through runtime.tls_g from go1.21; arm64 used the x1/x2 scratch pair in go1.17, x16/x17 with a
# compare through x17 in go1.18, and compares sp directly from go1.19). go1.17 is the oldest release
# that supports windows/arm64.
#
# Output (relative to the binaries repo root), <goversion> being one of GO_VERSIONS below:
#   tests/i386/langdetect_go[_<goversion>]                linux/386
#   tests/i386/windows/langdetect_go[_<goversion>].exe    windows/386
#   tests/armel/langdetect_go[_<goversion>]               linux/arm
#   tests/aarch64/langdetect_go[_<goversion>]             linux/arm64
#   tests/aarch64/windows/langdetect_go[_<goversion>].exe windows/arm64
#   tests/x86_64/windows/langdetect_go.exe                windows/amd64
#   tests/aarch64/langdetect_go.macho                     darwin/arm64
#
# The unsuffixed name is the current toolchain (GO_CURRENT); older ones carry the version suffix.
# The linux/amd64 build lives in build.sh (tests/x86_64/langdetect_go).
#
# Requirements:
#   - one Go toolchain per version in GO_VERSIONS, in $GO_SDK_DIR/<goversion>/bin/go
#     (https://go.dev/dl/<goversion>.linux-amd64.tar.gz)
#
# Binaries are deliberately NOT stripped: the Go symbol table is the ground truth the tests validate
# against.
#
# Usage:
#   cd <binaries-repo-root>/tests_src/language_detector
#   GO_SDK_DIR=/opt/go-sdk ./build_go_cross.sh

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
SRC="$SCRIPT_DIR/langdetect_go.go"

GO_VERSIONS=(go1.17.13 go1.18.10 go1.20.14 go1.22.5 go1.27.1)
GO_CURRENT=go1.27.1
GO_SDK_DIR="${GO_SDK_DIR:-$HOME/sdk}"
export CGO_ENABLED=0 GOTOOLCHAIN=local

build() { # <go> <goos> <goarch> <output>
    local out="$ROOT/$4"
    mkdir -p "$(dirname "$out")"
    GOOS="$2" GOARCH="$3" "$1" build -o "$out" "$SRC" \
        && echo "  OK: $4" || echo "  FAIL: $4"
}

for version in "${GO_VERSIONS[@]}"; do
    go_bin="$GO_SDK_DIR/$version/bin/go"
    echo "Go version : $("$go_bin" version)"
    # the current toolchain owns the unsuffixed names
    if [ "$version" = "$GO_CURRENT" ]; then suffix=""; else suffix="_$version"; fi

    build "$go_bin" linux   386   "tests/i386/langdetect_go$suffix"
    build "$go_bin" windows 386   "tests/i386/windows/langdetect_go$suffix.exe"
    build "$go_bin" linux   arm   "tests/armel/langdetect_go$suffix"
    build "$go_bin" linux   arm64 "tests/aarch64/langdetect_go$suffix"
    build "$go_bin" windows arm64 "tests/aarch64/windows/langdetect_go$suffix.exe"
done

# windows/amd64 is cross-checked against Go binaries from elsewhere, so only the current build
build "$GO_SDK_DIR/$GO_CURRENT/bin/go" windows amd64 tests/x86_64/windows/langdetect_go.exe

# darwin/amd64 is left out: the Go linker emits LC_UNIXTHREAD there and cle cannot load it yet.
# -trimpath because this one was not built at /workspace like its siblings, and without it the
# binary would carry the builder's home directory.
GOOS=darwin GOARCH=arm64 "$GO_SDK_DIR/$GO_CURRENT/bin/go" build -trimpath \
    -o "$ROOT/tests/aarch64/langdetect_go.macho" "$SRC" \
    && echo "  OK: tests/aarch64/langdetect_go.macho" || echo "  FAIL: tests/aarch64/langdetect_go.macho"
