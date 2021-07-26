#!/usr/bin/env python

from os import SEEK_SET

from elftools.construct import Container
from elftools.elf.constants import P_FLAGS
from elftools.elf.elffile import ELFFile
import sys

'''
This file is a workaround for a change in the linux kernel behavior. It used to be that whenever your stack was executable
so would all the data sections in your binary and libraries. This is no longer true, it now only makes the stack itself
executable. This forces us to manually patch all writable LOAD segments to be executable if the stack is so. 

Otherwise, e.g. the network_overflow testcase is only exploitable with the `call_shellcode` technique on older kernels, 
since it relied on copying the shellcode into the data section, which is no longer executable by default.
'''
patches = []
with open(sys.argv[1], 'rb') as f:
    elf = ELFFile(f)

    segs = [seg for seg in elf.iter_segments()]
    is_stack_exec = False
    for seg in segs:
        if seg.header.p_type != 'PT_GNU_STACK':
            continue
        if seg.header.p_flags & P_FLAGS.PF_X:
            is_stack_exec = True
    if not is_stack_exec:
        # nothing to do, the stack isn't executable anyways
        sys.exit(0)

    for i, seg in enumerate(elf.iter_segments()):
        if seg.header.p_type != 'PT_LOAD':
            continue

        if P_FLAGS.PF_W & seg.header.p_flags == 0:
            continue

        # found writable load segment, patch the flags to have PF_X set
        off = elf.header.e_phoff + elf.structs.Elf_Phdr.sizeof() * i

        x = seg.header.copy()
        x.p_flags |= P_FLAGS.PF_X
        new_header = elf.structs.Elf_Phdr.build(x)
        old_header = elf.structs.Elf_Phdr.build(seg.header)
        print(f"Patching segment #{i} of {sys.argv[1]} to be executable")
        patches.append((off, old_header, new_header))

with open(sys.argv[1], 'r+b') as f:
    for off, old_data, new_data in patches:
        f.seek(off, SEEK_SET)
        old = f.read(len(old_data))
        assert old == old_data
        f.seek(off, SEEK_SET)
        f.write(new_data)

