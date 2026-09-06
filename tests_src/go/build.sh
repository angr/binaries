#!/usr/bin/env bash
# Builds the Go decompiler corpus: every *.go here is a standalone package-main program.
#
# Output (relative to the binaries repo root), <goversion> being one of GO_VERSIONS:
#   tests/x86_64/go/<goversion>/<prog>            -gcflags=all=-l            (no inlining, optimized)
#   tests/x86_64/go/<goversion>/<prog>_N          -gcflags='all=-N -l'       (no inlining, no optimization)
#   tests/x86_64/go/<goversion>/<prog>_stripped   -gcflags=all=-l -ldflags='-s -w' (symbols only via pclntab)
#   tests/aarch64/go/<goversion>/<prog>[_stripped] arm64 builds of ARM64_PROGS (default: basics)
#   tests/x86_64/go/go1.27.1/<prog>_inlined        default inlining (INLINED_PROGS; shapes that need it)
#   tests/i386/go/go1.27.1/<prog>                  386 builds of I386_PROGS, -gcflags=all=-l
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
PROGS=${PROGS:-"basics builtins conc iface maps swap"}

# arm64 builds (optimized + stripped only) of the programs in ARM64_PROGS land under tests/aarch64/go/<goversion>/
ARM64_PROGS=${ARM64_PROGS:-"basics"}
# programs whose shape only appears with the inliner on (one optimized amd64 build, go1.27.1 only)
INLINED_PROGS=${INLINED_PROGS:-"uninit"}
# 386 builds (one optimized build, go1.27.1 only)
I386_PROGS=${I386_PROGS:-"recv"}

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

GO="$GO_SDK_DIR/go1.27.1/bin/go"
out="$ROOT/tests/x86_64/go/go1.27.1"
for prog in $INLINED_PROGS; do
    GOARCH=amd64 "$GO" build -o "$out/${prog}_inlined" "$prog.go"
    echo "built $prog (inlined) with go1.27.1"
done
out="$ROOT/tests/i386/go/go1.27.1"
mkdir -p "$out"
for prog in $I386_PROGS; do
    GOARCH=386 "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
    echo "built $prog (386) with go1.27.1"
done
