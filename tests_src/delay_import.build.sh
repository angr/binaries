#!/bin/sh
# Build delay_import.exe (a PE32+ with a delay-load import of user32.dll!MessageBoxA).
#
# GNU ld does not populate the PE delay-import data directory (entry 13), so we
# build with LLVM's lld MinGW driver, which can convert an import to delay-load
# only when it comes from a short-format import library. GNU dlltool produces
# the long format, so the import library is generated with llvm-dlltool.
#
# Requires: gcc-mingw-w64-x86-64, lld, llvm (for llvm-dlltool).
set -e

TRIPLE=x86_64-w64-mingw32
DLLTOOL=${LLVM_DLLTOOL:-$(command -v llvm-dlltool || command -v llvm-dlltool-14)}
GCCDIR=$(${TRIPLE}-gcc -print-libgcc-file-name | xargs dirname)
LIBDIR=${GCCDIR}/../../../../${TRIPLE}/lib

# 1. compile
${TRIPLE}-gcc -c delay_import.c -o delay_import.o

# 2. short-format import library for user32.dll
cat > user32.def <<DEF
LIBRARY user32.dll
EXPORTS
MessageBoxA
DEF
"${DLLTOOL}" -m i386:x86-64 -d user32.def -l libuser32.delay.a

# 3. link with lld, delay-loading user32.dll
ld.lld -m i386pep -Bdynamic --strip-all --delayload=user32.dll -o delay_import.exe \
    "${LIBDIR}/crt2.o" "${GCCDIR}/crtbegin.o" -L. -L"${GCCDIR}" -L"${LIBDIR}" \
    delay_import.o ./libuser32.delay.a \
    -lmingw32 -lgcc -lgcc_eh -lmoldname -lmingwex -lmsvcrt -lkernel32 -ladvapi32 -lshell32 -lkernel32 \
    "${GCCDIR}/crtend.o"

rm -f delay_import.o user32.def libuser32.delay.a
