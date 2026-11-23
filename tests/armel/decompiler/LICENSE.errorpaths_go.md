# errorpaths_go fixture provenance

`errorpaths_go` is a Go 1.27.0 program compiled for `linux/arm`. The ELF sets
neither ARM float-ABI flag (`e_flags 0x5000002`, EABI version 5), so it loads as
ARMEL. It was built with:

```text
GOOS=linux GOARCH=arm go build -trimpath -ldflags="-s -w"
```

The committed binary is 1,471,656 bytes, mode 0755, and has SHA-256
`a954af2c614288d6d5985c2bec00bcc0f46d8bd96d4a34eb868e42d15d9cf875`.
It was built with `-trimpath`, so it embeds no build paths, and with `-s -w`,
so it retains no DWARF; function names come from its `.gopclntab`.

The program source is a deterministically generated error-handling test program,
distributed under the MIT license and redistributable.

As a statically linked Go binary it also contains the Go runtime and standard
library, which the Go project distributes under the BSD 3-Clause license:

<https://github.com/golang/go/blob/go1.27.0/LICENSE>

The decompiler fixture target is `runtime.boundsError.Error` at `0x2744c`, which
is Go runtime code.
