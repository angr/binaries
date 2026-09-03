#!/usr/bin/env bash
# Builds the Go decompiler corpus: every *.go here is a standalone package-main program.
#
# Output (relative to the binaries repo root), <goversion> being one of GO_VERSIONS:
#   tests/x86_64/go/<goversion>/<prog>            -gcflags=all=-l            (no inlining, optimized)
#   tests/x86_64/go/<goversion>/<prog>_N          -gcflags='all=-N -l'       (no inlining, no optimization)
#   tests/x86_64/go/<goversion>/<prog>_stripped   -gcflags=all=-l -ldflags='-s -w' (symbols only via pclntab)
#   tests/aarch64/go/<goversion>/<prog>[_stripped] arm64 builds of ARM64_PROGS (default: basics)
#
# Requirements: one Go toolchain per version in $GO_SDK_DIR/<goversion>/bin/go
# (https://go.dev/dl/<goversion>.linux-amd64.tar.gz).
#
# Usage:
#   cd <binaries-repo-root>/tests_src/go
#   GO_SDK_DIR=/path/to/sdks ./build.sh            # all programs, all versions
#   PROGS="basics" GO_VERSIONS="go1.22.5" ./build.sh
set -euo pipefail

cd "$(dirname "$0")"
ROOT=$(git rev-parse --show-toplevel)
GO_SDK_DIR=${GO_SDK_DIR:-/workspace/tools}
GO_VERSIONS=${GO_VERSIONS:-"go1.22.5 go1.27.1"}
PROGS=${PROGS:-$(ls ./*.go | sed 's#^\./##; s/\.go$//')}

# arm64 builds (optimized + stripped only) of the programs in ARM64_PROGS land under tests/aarch64/go/<goversion>/
ARM64_PROGS=${ARM64_PROGS:-"basics"}

export CGO_ENABLED=0 GOOS=linux GOFLAGS=-trimpath

for ver in $GO_VERSIONS; do
    GO="$GO_SDK_DIR/$ver/bin/go"
    out="$ROOT/tests/x86_64/go/$ver"
    mkdir -p "$out"
    for prog in $PROGS; do
        GOARCH=amd64 "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
        GOARCH=amd64 "$GO" build -gcflags='all=-N -l' -o "$out/${prog}_N" "$prog.go"
        GOARCH=amd64 "$GO" build -gcflags=all=-l -ldflags='-s -w' -o "$out/${prog}_stripped" "$prog.go"
        echo "built $prog with $ver"
    done
    out="$ROOT/tests/aarch64/go/$ver"
    mkdir -p "$out"
    for prog in $ARM64_PROGS; do
        GOARCH=arm64 "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
        GOARCH=arm64 "$GO" build -gcflags=all=-l -ldflags='-s -w' -o "$out/${prog}_stripped" "$prog.go"
        echo "built $prog (arm64) with $ver"
    done
done
