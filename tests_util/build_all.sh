#!/bin/bash

SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
NAME=$1

if [ "$(basename `pwd`)" != "tests_src" ]; then
	echo "This script has to be run inside the tests_src directory!"
	exit 1
fi

CFLAGS=""
if [ "$NAME" == "network_overflow" ]; then
	echo "WARNING: making the stack executable for the network_overflow binary."
	CFLAGS="$CFLAGS -z execstack -fno-stack-protector"
fi
powerpc64-linux-gnu-gcc $CFLAGS -O0 $NAME.c -o ../tests/ppc64/$NAME
powerpc-linux-gnu-gcc $CFLAGS -O0 $NAME.c -o ../tests/ppc/$NAME
powerpc64le-linux-gnu-gcc $CFLAGS -O0 $NAME.c -o ../tests/ppc64el/$NAME
mipsel-linux-gnu-gcc $CFLAGS -O0 $NAME.c -o ../tests/mipsel/$NAME
#mips-malta-linux-gnu-gcc -O0 $NAME.c -o ../tests/mips/$NAME
mips-linux-gnu-gcc $CFLAGS -O0 $NAME.c -o ../tests/mips/$NAME
arm-linux-gnueabihf-gcc $CFLAGS -O0 $NAME.c -o ../tests/armhf/$NAME
arm-linux-gnueabi-gcc $CFLAGS -O0 $NAME.c -o ../tests/armel/$NAME
gcc $CFLAGS -m32 -O0 $NAME.c -o ../tests/i386/$NAME
gcc $CFLAGS -O0 $NAME.c -o ../tests/x86_64/$NAME

if [ "$NAME" = "network_overflow" ]; then
	echo "WARNING: Running the make_data_executable.py script on the resulting binaries to ensure the call_shellcode technique testcases work!!!";
	sleep 1;
	for f in ../tests/*/$NAME; do 
		"$SCRIPT_DIR/make_data_executable.py" "$f"
	done
fi
