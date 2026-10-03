#!/bin/sh
# Build the two LC_UNIXTHREAD variants of the tracked tests/x86_64/terramate.macho.
#
# terramate.macho carries its entry point in an LC_UNIXTHREAD command whose header sits at
# file offset 0x650: cmd 0x5, cmdsize 184, flavor 4 (x86_THREAD_STATE64), count 42 words of
# thread state. Each variant rewrites one 32-bit field of that header and nothing else, so
# each is a single changed byte and all three files are the same length.
#
#   terramate_float_thread_state.macho   flavor 4 -> 5 at 0x658. Flavor 5 is x86_FLOAT_STATE64,
#       a thread state LC_UNIXTHREAD is allowed to carry and that holds no program counter, so
#       a loader has nothing in it to take an entry point from and must still load the rest.
#
#   terramate_short_thread_state.macho   count 42 -> 2 at 0x65c. Two 32-bit words is far short
#       of an x86_thread_state64_t, so a loader that unpacked the flavor's layout anyway would
#       read 136 bytes where the command declares 8, and report as the program counter a word
#       that is not part of the thread state the command describes. cmdsize is untouched, so
#       those bytes are still inside the command and inside the file.
#
# Nothing else in this repository has either shape: terramate.macho is the only Mach-O here
# that carries an LC_UNIXTHREAD at all.
set -eu

here=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
root=$here/../..
src=$root/tests/x86_64/terramate.macho

python3 - "$src" "$root/tests/x86_64" <<'PY'
import hashlib
import struct
import sys

src, outdir = sys.argv[1], sys.argv[2]
UNIXTHREAD_OFFSET = 0x650
LC_UNIXTHREAD = 0x5

data = open(src, "rb").read()
cmd, cmdsize, flavor, count = struct.unpack_from("<4I", data, UNIXTHREAD_OFFSET)
assert (cmd, cmdsize, flavor, count) == (LC_UNIXTHREAD, 184, 4, 42), (cmd, cmdsize, flavor, count)

for name, field, value in (
    ("terramate_float_thread_state.macho", 8, 5),
    ("terramate_short_thread_state.macho", 12, 2),
):
    out = bytearray(data)
    struct.pack_into("<I", out, UNIXTHREAD_OFFSET + field, value)
    out = bytes(out)
    with open(f"{outdir}/{name}", "wb") as fh:
        fh.write(out)
    print(hashlib.sha256(out).hexdigest(), name)
PY
