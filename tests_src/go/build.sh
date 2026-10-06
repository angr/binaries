#!/usr/bin/env bash
# Builds the Go decompiler corpus: every *.go here is a standalone package-main program.
#
# Output (relative to the binaries repo root), <goversion> being one of GO_VERSIONS:
#   tests/x86_64/go/<goversion>/<prog>            -gcflags=all=-l            (no inlining, optimized)
#   tests/x86_64/go/<goversion>/<prog>_N          -gcflags='all=-N -l'       (no inlining, no optimization)
#   tests/x86_64/go/<goversion>/<prog>_stripped   -gcflags=all=-l -ldflags='-s -w' (symbols only via pclntab)
#   tests/aarch64/go/<goversion>/<prog>[_stripped] arm64 builds of ARM64_PROGS (default: basics)
#   tests/<arch>/go/go1.27.1/<prog>...               GO127_PROGS (need go1.23+ APIs): amd64 (three builds), arm64, 386
#   tests/x86_64/go/go1.27.1/<prog>_inlined        default inlining (INLINED_PROGS; shapes that need it)
#   tests/i386/go/go1.27.1/<prog>                  386 builds of I386_PROGS, -gcflags=all=-l
#   tests/armel/go/go1.27.1/<prog>                 32-bit arm builds of ARM_PROGS, -gcflags=all=-l
#   tests/x86_64/go/go1.22.5/<prog>                GO122_PROGS (shapes go1.23+ no longer emits), -gcflags=all=-l
#   tests/x86_64/go/<goversion>/basics[_stripped]  LEGACY_VERSIONS (pre-1.17 pclntab layouts: go1.4.3,
#                                                  go1.9.7, go1.10.8, go1.15.15, go1.16.15), amd64 only
#   tests/x86_64/go/go1.16.15/basics_pie[_extld]_stripped  PIE_VERSION -buildmode=pie, stripped; the pre-1.18
#                                                  table lives in relro: .data.rel.ro.gopclntab with the
#                                                  internal linker, merged into .data.rel.ro by an
#                                                  external one (_extld, needs cgo and a C toolchain)
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
# programs that need go1.23+ APIs (sync/atomic And/Or), built with go1.27.1 only
GO127_PROGS=${GO127_PROGS:-"atomics typeswitch"}
# programs whose shape only appears with the inliner on (one optimized amd64 build, go1.27.1 only)
INLINED_PROGS=${INLINED_PROGS:-"uninit strvals"}
# 386 builds (one optimized build, go1.27.1 only)
I386_PROGS=${I386_PROGS:-"recv"}
# 32-bit arm builds (one optimized build, go1.27.1 only)
ARM_PROGS=${ARM_PROGS:-"maps"}
# programs whose shape needs go1.22 or older (one optimized amd64 build, go1.22.5 only)
GO122_PROGS=${GO122_PROGS:-"defers"}
# toolchains with older pclntab layouts: basics only, optimized and stripped; the empty default keeps
# a plain ./build.sh from needing them
LEGACY_VERSIONS=${LEGACY_VERSIONS:-""}
# toolchain for the PIE builds; empty by default like LEGACY_VERSIONS
PIE_VERSION=${PIE_VERSION:-""}

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
for prog in $GO127_PROGS; do
    out="$ROOT/tests/x86_64/go/go1.27.1"
    GOARCH=amd64 "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
    GOARCH=amd64 "$GO" build -gcflags='all=-N -l' -o "$out/${prog}_N" "$prog.go"
    GOARCH=amd64 "$GO" build -gcflags=all=-l -ldflags='-s -w' -o "$out/${prog}_stripped" "$prog.go"
    out="$ROOT/tests/aarch64/go/go1.27.1"
    GOARCH=arm64 "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
    GOARCH=arm64 "$GO" build -gcflags=all=-l -ldflags='-s -w' -o "$out/${prog}_stripped" "$prog.go"
    out="$ROOT/tests/i386/go/go1.27.1"
    mkdir -p "$out"
    GOARCH=386 "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
    echo "built $prog (amd64, arm64, 386) with go1.27.1"
done
out="$ROOT/tests/i386/go/go1.27.1"
mkdir -p "$out"
for prog in $I386_PROGS; do
    GOARCH=386 "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
    echo "built $prog (386) with go1.27.1"
done
out="$ROOT/tests/armel/go/go1.27.1"
mkdir -p "$out"
for prog in $ARM_PROGS; do
    GOOS=linux GOARCH=arm "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
    echo "built $prog (arm) with go1.27.1"
done

GO="$GO_SDK_DIR/go1.22.5/bin/go"
out="$ROOT/tests/x86_64/go/go1.22.5"
mkdir -p "$out"
for prog in $GO122_PROGS; do
    GOARCH=amd64 "$GO" build -gcflags=all=-l -o "$out/$prog" "$prog.go"
    echo "built $prog with go1.22.5"
done

# -trimpath only exists from go1.13, so the older builds carry the build directory in their file
# names; the all= pattern in -gcflags from go1.10; go1.4 needs GOROOT to find itself.
for ver in $LEGACY_VERSIONS; do
    GO="$GO_SDK_DIR/$ver/bin/go"
    out="$ROOT/tests/x86_64/go/$ver"
    mkdir -p "$out"
    minor=${ver#go1.}; minor=${minor%%.*}
    if [ "$minor" -ge 10 ]; then gcflags="all=-l"; else gcflags="-l"; fi
    GOROOT="$GO_SDK_DIR/$ver" GOARCH=amd64 GOFLAGS= "$GO" build -gcflags=$gcflags -o "$out/basics" basics.go
    GOROOT="$GO_SDK_DIR/$ver" GOARCH=amd64 GOFLAGS= "$GO" build -gcflags=$gcflags -ldflags='-s -w' -o "$out/basics_stripped" basics.go
    echo "built basics (legacy) with $ver"
done

if [ -n "$PIE_VERSION" ]; then
    GO="$GO_SDK_DIR/$PIE_VERSION/bin/go"
    out="$ROOT/tests/x86_64/go/$PIE_VERSION"
    mkdir -p "$out"
    GOROOT="$GO_SDK_DIR/$PIE_VERSION" GOARCH=amd64 "$GO" build -buildmode=pie -gcflags=all=-l -ldflags='-s -w' \
        -o "$out/basics_pie_stripped" basics.go
    CGO_ENABLED=1 GOROOT="$GO_SDK_DIR/$PIE_VERSION" GOARCH=amd64 "$GO" build -buildmode=pie -gcflags=all=-l \
        -ldflags='-linkmode=external -s -w' -o "$out/basics_pie_extld_stripped" basics.go
    echo "built basics (pie) with $PIE_VERSION"
fi
