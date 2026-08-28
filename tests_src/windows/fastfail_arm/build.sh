#!/usr/bin/env bash
# Rebuild the Windows ARM fastfail fixtures.
#
# Freestanding: no CRT, no imports, no Windows SDK, so this needs only clang and
# lld-link. Both are produced from the same source; only the target differs.
set -euo pipefail
cd "$(dirname "$0")"
for spec in aarch64-unknown-windows-msvc:arm64:fastfail_arm64.exe \
            thumbv7-unknown-windows-msvc:arm:fastfail_armnt.exe; do
    target=${spec%%:*}; rest=${spec#*:}; machine=${rest%%:*}; out=${rest##*:}
    clang --target="$target" -c -O1 -o fastfail.obj fastfail.c
    lld-link /entry:mainCRTStartup /subsystem:console "/machine:$machine" "/out:$out" fastfail.obj
done
rm -f fastfail.obj
