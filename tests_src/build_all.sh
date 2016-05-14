#!/bin/bash

NAME=$1

powerpc64-unknown-linux-gnu-gcc -O0 $NAME.c -o ../tests/ppc64/$NAME
powerpc-linux-gnu-gcc-4.8 -O0 $NAME.c -o ../tests/ppc/$NAME
mipsel-unknown-linux-gnu-gcc -O0 $NAME.c -o ../tests/mipsel/$NAME
#mips-malta-linux-gnu-gcc -O0 $NAME.c -o ../tests/mips/$NAME
mips-unknown-linux-gnu-gcc -O0 $NAME.c -o ../tests/mips/$NAME
arm-linux-gnueabihf-gcc-4.8 -O0 $NAME.c -o ../tests/armhf/$NAME
arm-linux-gnueabi-gcc -O0 $NAME.c -o ../tests/armel/$NAME
gcc -m32 -O0 $NAME.c -o ../tests/i386/$NAME
gcc -O0 $NAME.c -o ../tests/x86_64/$NAME
