# s390x EXRL out-of-buffer read

`acme-client` is the s390x build of acme-client 1.3.5, taken from `usr/bin/`
of Alpine v3.23's community package `acme-client-1.3.5-r0`:

    https://dl-cdn.alpinelinux.org/alpine/v3.23/community/s390x/acme-client-1.3.5-r0.apk

sha256 `5a78486413d0583c8f835a79edd32e4189e6218ef0a6894d42e0ec3aede560d3`,
79856 bytes, ELF64 big-endian PIE for IBM S/390, dynamically linked against musl.
The `.apk` it came from is sha256
`cf0789058b4b81d04ae3a0a23a0a54644030691071b29fc9cc82dea79da385fb`.

It is here because lifting it kills the process. `CFGFast` scans the executable
segment for code and lifts at image offset `0x723a`, which is not on any
instruction path. The entry stub runs from `0x7210` to `0x7233` and branches
straight to `0x7240`, and in between sit alignment padding at `0x7234`, the
four-byte offset word at `0x7238` that the stub loads to find `_DYNAMIC`
(`0x7238 + 0xc660 = 0x13898`), and four more padding bytes at `0x723c`. The scan
lands two bytes into that data word, so the six bytes it reads are the low half
of the word followed by the padding:

    0x723a: c6 60 07 07 07 07

which decode as `exrl %r6` with an RI2 of `0x07070707`, a target 235,802,126
bytes ahead.

`s390_irgen_EXRL` in libVEX reads that target straight out of the buffer it is
translating, at `exrl_bytes + offset * 2`, with nothing checking that the result
is still inside the buffer. cle hands over the whole 73,920-byte executable
segment, so the read lands 235,757,448 bytes -- about 225 MiB -- past the end of
it and the process takes `SIGSEGV`. angr maps this PIE at `0x400000`, so the
address in a traceback is `0x40723a`.

acme-client is GPL-2.0-only; redistribution is allowed.
