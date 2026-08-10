# Breakpad 64-bit Windows minidump

`tiny-exe-with-cet-xsave.dmp` is an unmodified upstream Breakpad test fixture: a
64-bit Windows minidump of a small crashing program, written by Crashpad on a
machine with CET shadow stacks, so the thread context is saved through XSAVE.

It is the smallest real AMD64 Windows dump on hand whose thread state can be
read back once its module list is set aside, which is what `cle`'s minidump
tests need to cover a dump that lists no modules on the architecture where the
module list decides between AMD64 and WoW64 x86.

- Source: https://github.com/google/breakpad/blob/69e9aada412e81575a95d0d94f4592fe1b8dfc15/src/processor/testdata/tiny-exe-with-cet-xsave.dmp
- Upstream commit: `69e9aada412e81575a95d0d94f4592fe1b8dfc15`
- SHA-256: `2e852e97737158a795ea4b588addd95c9ece2a6c84ba4f77a82fb44d008709a7`
- License: BSD-3-Clause

Copyright 2006 Google LLC

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.
3. Neither the name of Google LLC nor the names of its contributors may be used
   to endorse or promote products derived from this software without specific
   prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
