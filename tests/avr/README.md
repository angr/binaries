# AVR static archive

`libgcov_avr31.a` is `usr/lib/gcc/avr/14.2.0/avr31/libgcov.a` from Debian Ports'
`gcc-avr` 1:14.2.0-2, the `hurd-i386` build:

    https://snapshot.debian.org/archive/debian-ports/20241102T074343Z/pool-hurd-i386/main/g/gcc-avr/gcc-avr_14.2.0-2_hurd-i386.deb

The `.deb` is sha256
`d1a5db88c0fc04b518ec0c70e50d85644e357a957b33205e9b18e75f34796342`. `gcc-avr` is
a cross compiler, so the package architecture is `hurd-i386` while everything
inside this archive is AVR.

Take the file from the Debian **Ports** archive and not from Debian main. Main
carries the same version built for amd64
(`https://deb.debian.org/debian/pool/main/g/gcc-avr/gcc-avr_14.2.0-2_amd64.deb`),
and its copy of this archive is also 31660 bytes but hashes to
`e9fd418d99dda7c0c34da108dc4fc143add544c5ba5d72128c93ec0597a8347b`. The two
differ in exactly 230 bytes, every one of them in an `ar` header -- the buildd's
uid and gid and the index mtime. No member body byte differs.

sha256 `63ffbb28b90916b17ceb446eea8fef3c5523c4f719c5642324fbc72762f27e0f`,
31660 bytes. It is an `ar` archive holding a symbol index, a long-name string
table and 28 ELF32 little-endian relocatable objects for AVR (`EM_AVR`,
`ET_REL`, `e_flags` `0x9f`, so `EF_AVR_MACH` 31 -- the `avr31` multilib -- with
`EF_AVR_LINKRELAX_PREPARED` set). All 28 were produced by GCC 14.2.0. On disk
the members run from 620 bytes (`0x26c`) to 11,096 bytes (`0x2b58`): 21 members
of 620 bytes, four of 848, one of 844, one of 852, and the 11,096-byte `libgcov`
driver object.

It is here so cle can be tested on where archive members land in a 16-bit
address space. AVR code addresses are 16 bits wide, so an archive of this shape
has far more members than will fit if each one is given a generously rounded
slot, and the fixture makes that placement observable on a real toolchain's
output rather than a synthesised one.

`libgcov` is part of the GCC runtime. Upstream, the sources it is built from
(`libgcc/libgcov.h`, `libgcc/libgcov-driver.c` and `libgcc/libgcov-interface.c`
at `releases/gcc-14.2.0`) are GPL-3.0-or-later WITH GCC-exception-3.1. The
Debian package's own `usr/share/doc/gcc-avr/copyright` records the coarser
`GPL-3+` and does not mention the exception. Redistribution is allowed under
either reading.
