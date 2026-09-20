# DecBench `nologin` fixture

`nologin` is the stripped x86-64 ELF used by the DecBench `O0/shadow/nologin`
case. It was derived from the published dataset at revision
`e5eb576d66ee36793b800a4dd45e291e0add4472`:

- [source binary](https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/e5eb576d66ee36793b800a4dd45e291e0add4472/binaries/O0/shadow/nologin)
- [source C](https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/e5eb576d66ee36793b800a4dd45e291e0add4472/sources/shadow/nologin.c)

The published binary has SHA-256
`e5f129967f4cc6980359cee7bf892f104edd183848507f8b30c5d808f7999b40`.
Running GNU Binutils 2.46 `strip --strip-all` on it produces this fixture
byte-for-byte, with SHA-256
`e368a4f622a05943dcc1cdb1fb0d7a22e79b894794489b17b547e68230eaa38d`.

The DecBench dataset declares the [BSD 2-Clause
license](https://huggingface.co/datasets/noelo-lab/decbench-dataset/blob/e5eb576d66ee36793b800a4dd45e291e0add4472/README.md#L1-L7).
The underlying source is shadow 4.13's [`src/nologin.c` at commit
`24b44b686ea2221405cfd806dede046cf1fd9584`](https://github.com/shadow-maint/shadow/blob/24b44b686ea2221405cfd806dede046cf1fd9584/src/nologin.c),
which carries the following BSD 2-Clause notice and license:

```text
Copyright 2004 The FreeBSD Project.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.

2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
```
