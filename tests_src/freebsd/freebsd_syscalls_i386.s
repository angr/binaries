# A FreeBSD/i386 program that issues its system calls the way FreeBSD takes them:
# the number goes in eax and the arguments are pushed on the stack. The kernel reads
# them from tf_esp + sizeof(uint32_t) -- past the slot a call to a libc stub would have
# left the return address in -- so a program issuing int 0x80 by hand pushes one word
# there first. Pushing eax is the idiom; the value does not matter.
#
# Numbers are FreeBSD's own, from sys/sys/syscall.h: 1 exit, 4 write, 5 open, 6 close,
# 33 access, 128 rename, 272 getdents. Linux's i386 table carries the same name for the
# five below 34 -- 1, 4, 5, 6 and 33 -- because both systems inherited them from Research
# Unix: 1, 4, 5 and 6 are already in V5's sysent.c and 33 arrives in V7 (the Research-V5
# and Research-V7 tags of dspinellis/unix-history-repo). It means something else by the
# other two: 128 is init_module there and 272 is fadvise64_64.
#
# Built with:
#   as --32 -o freebsd_syscalls_i386.o freebsd_syscalls_i386.s
#   ld -m elf_i386 -o freebsd-syscalls-i386 freebsd_syscalls_i386.o
#
# and then branded. On FreeBSD that is `brandelf -t FreeBSD freebsd-syscalls-i386`;
# brandelf is not packaged in this workspace, so ELFOSABI_FREEBSD was written into
# e_ident[EI_OSABI] directly, which is all brandelf -t does -- see
# contrib/elftoolchain/brandelf/brandelf.c, which assigns that one byte and nothing else.
#
# The object file's name reaches the binary: GNU as records its -o argument as the
# STT_FILE symbol, so the commands above have to be run with these exact names to
# reproduce the committed bytes.

        .section .rodata
path:   .asciz "/COPYRIGHT"
dir:    .asciz "/"
from:   .asciz "/tmp/from"
to:     .asciz "/tmp/to"
banner: .ascii "FreeBSD\n"
banner_len = . - banner

        .section .bss
        .lcomm  entries, 512

        .section .text
        .globl _start
_start:
        # access("/COPYRIGHT", 0)
        pushl   $0
        pushl   $path
        movl    $33, %eax
        pushl   %eax
        int     $0x80
        addl    $12, %esp
        testl   %eax, %eax
        jnz     done

        # open("/", 0, 0)
        pushl   $0
        pushl   $0
        pushl   $dir
        movl    $5, %eax
        pushl   %eax
        int     $0x80
        addl    $16, %esp
        movl    %eax, %ebx

        # write(1, banner, banner_len)
        pushl   $banner_len
        pushl   $banner
        pushl   $1
        movl    $4, %eax
        pushl   %eax
        int     $0x80
        addl    $16, %esp

        # getdents(fd, entries, sizeof entries)
        pushl   $512
        pushl   $entries
        pushl   %ebx
        movl    $272, %eax
        pushl   %eax
        int     $0x80
        addl    $16, %esp

        # close(fd)
        pushl   %ebx
        movl    $6, %eax
        pushl   %eax
        int     $0x80
        addl    $8, %esp

        # rename("/tmp/from", "/tmp/to")
        pushl   $to
        pushl   $from
        movl    $128, %eax
        pushl   %eax
        int     $0x80
        addl    $12, %esp

done:
        # exit(0)
        pushl   $0
        movl    $1, %eax
        pushl   %eax
        int     $0x80
        hlt
