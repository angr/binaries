#!/bin/sh
# Build tests/x86/windows/TLS_huge_zero_fill.exe from the tracked tests/x86/windows/TLS.exe.
#
# The only difference is IMAGE_TLS_DIRECTORY32.SizeOfZeroFill, the little-endian dword at file
# offset 0x68ec, which goes from 0 to 0xf0000000. The low three bytes are already zero, so on
# disk this rewrites the single byte at 0x68ef from 0x00 to 0xf0 and the length does not change.
set -eu

here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
root=$here/../..
src=$root/tests/x86/windows/TLS.exe
dst=$root/tests/x86/windows/TLS_huge_zero_fill.exe

python3 - "$src" "$dst" <<'PY'
import hashlib
import sys

src, dst = sys.argv[1], sys.argv[2]
data = bytearray(open(src, "rb").read())
assert data[0x68EC:0x68F0] == b"\0\0\0\0", data[0x68EC:0x68F0].hex()
data[0x68EC:0x68F0] = (0xF0000000).to_bytes(4, "little")
out = bytes(data)
open(dst, "wb").write(out)
print(hashlib.sha256(out).hexdigest(), dst)
PY
