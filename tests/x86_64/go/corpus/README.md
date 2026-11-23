Real-world Go builds from the gors-bins `real-world-v1` corpus (go1.27.0, `-trimpath`, no DWARF, no symbol table;
names come from the pclntab). `age-keygen-v1.3.1-linux-amd64` (BSD-3-Clause, filippo.io/age) and
`../../../aarch64/go/corpus/age-v1.3.1-darwin-arm64` back angr's `test_go_corpus.py` regression on stripped binaries.

`u-root-v0.16.0-darwin-amd64` and `u-root-v0.16.0-windows-amd64.exe` are github.com/u-root/u-root v0.16.0
(BSD-3-Clause, https://github.com/u-root/u-root/blob/v0.16.0/LICENSE), built with go1.27.0, `-trimpath`,
`-ldflags='-s -w'` and `CGO_ENABLED=0`. The first Go release name in their read-only data is `go1.9`; the build-info
blob says `go1.27.0`. They back angr's `test_go_calling_convention.py` check that the release is read from the blob.
