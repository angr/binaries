#!/usr/bin/env bash
# Cross-compiled Go variants of the language detector, for CFG tests that need a Go binary of a
# specific architecture. Pure Go with CGO disabled cross-compiles with GOOS/GOARCH alone.
#
# Output (relative to the binaries repo root):
#   tests/i386/langdetect_go             linux/386
#   tests/i386/windows/langdetect_go.exe windows/386
#   tests/x86_64/windows/langdetect_go.exe windows/amd64
#   tests/armel/langdetect_go            linux/arm
#   tests/aarch64/langdetect_go          linux/arm64
#
# The linux/amd64 build lives in build.sh (tests/x86_64/langdetect_go).
#
# Requirements:
#   - go (set GO_BIN to override path, e.g. GO_BIN=/opt/go/bin/go)
#
# The committed binaries were built with go1.27.1. Binaries are deliberately NOT stripped: the Go
# symbol table is the ground truth the tests validate against.
#
# Usage:
#   cd <binaries-repo-root>/tests_src/language_detector
#   ./build_go_cross.sh

set -euo pipefail

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/../.." && pwd)"
SRC="$SCRIPT_DIR/langdetect_go.go"

GO_BIN="${GO_BIN:-go}"
export CGO_ENABLED=0

build() { # <goos> <goarch> <output>
    local out="$ROOT/$3"
    mkdir -p "$(dirname "$out")"
    GOOS="$1" GOARCH="$2" "$GO_BIN" build -o "$out" "$SRC" \
        && echo "  OK: $3" || echo "  FAIL: $3"
}

echo "Go version : $("$GO_BIN" version)"

build linux   386   tests/i386/langdetect_go
build windows 386   tests/i386/windows/langdetect_go.exe
build windows amd64 tests/x86_64/windows/langdetect_go.exe
build linux   arm   tests/armel/langdetect_go
build linux   arm64 tests/aarch64/langdetect_go
