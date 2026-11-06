# Deliberately malformed `/SYM64/` archives

`sym64_archive.a` is a real MIPS64 big-endian static library whose index is the
64-bit `/SYM64/` symbol table. The two files named after it are copies with one
field of that symbol table's member header overwritten, so a loader test can pin
how a malformed archive is reported without building an archive while it runs.

An ar member header is 60 bytes: 16 of name, 12 + 6 + 6 + 8 of metadata, 10 of
decimal size, then a two-byte magic. The symbol table is the archive's first
member, so its header starts at offset 8, after the `!<arch>\n` global header.

| file | bytes rewritten | what that breaks |
| --- | --- | --- |
| `sym64_archive_bad_magic.a` | 66..68 to `XX` | the member header's terminating magic |
| `sym64_archive_bad_size.a` | 56..66 to `999999999 ` | the symbol table's size, which now ends past this 3,104-byte file |

Each differs from `sym64_archive.a` in those bytes and nowhere else, and all
three files are 3,104 bytes.

- `sym64_archive.a`: SHA-256 `992dec1ee6f8400f30759f7998d7f3699261a39f7d7c03d175f1d5f7269e5782`
- `sym64_archive_bad_magic.a`: SHA-256 `19ad7469a1b07d66cb3b8c1c202b013d23e49b8b7d12aa64a435a21dcd4caf66`
- `sym64_archive_bad_size.a`: SHA-256 `1549bfdeb2385ba7a88147c399ae619b9ea69c257a6dd4e8d9047c5f3c6c7527`
