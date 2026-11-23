#!/bin/sh
# Build tests/x86_64/fauxware_oversized_segment.macho from the tracked tests/x86_64/fauxware.macho.
#
# The only difference is the filesize of the __DATA LC_SEGMENT_64 command, the little-endian
# qword at file offset 0x270, which goes from 0x1000 to 0x2000. The other seven bytes are
# already what 0x2000 needs, so on disk this rewrites the single byte at 0x271 from 0x10 to
# 0x20 and the length does not change.
#
# __DATA still maps vmsize 0x1000 at 0x100001000, so the segment now claims more file content
# than it has address space for. At 213d9d4, before this fixture, none of the 116
# LC_SEGMENT(_64) commands in the 28 Mach-O files this repository tracked had
# 0 < vmsize < filesize.
set -eu

here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
root=$here/../..
src=$root/tests/x86_64/fauxware.macho
dst=$root/tests/x86_64/fauxware_oversized_segment.macho

python3 - "$src" "$dst" <<'PY'
import hashlib
import sys

src, dst = sys.argv[1], sys.argv[2]
data = bytearray(open(src, "rb").read())
assert data[0x270:0x278] == (0x1000).to_bytes(8, "little"), data[0x270:0x278].hex()
data[0x270:0x278] = (0x2000).to_bytes(8, "little")
out = bytes(data)
open(dst, "wb").write(out)
print(hashlib.sha256(out).hexdigest(), dst)
PY
